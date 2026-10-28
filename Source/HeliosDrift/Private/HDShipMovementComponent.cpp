#include "HDShipMovementComponent.h"
#include "HDShipPowerComponent.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"
#include "Engine/World.h"

UHDShipMovementComponent::UHDShipMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	SetIsReplicatedByDefault(true);
}

void UHDShipMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHDShipMovementComponent, TravelMode);
	DOREPLIFETIME_CONDITION(UHDShipMovementComponent, ServerState, COND_SimulatedOnly);
}

float UHDShipMovementComponent::GetSpeedCap() const
{
	float Engines = 50.f;
	if (const AActor* Owner = GetOwner())
	{
		if (const UHDShipPowerComponent* Power = Owner->FindComponentByClass<UHDShipPowerComponent>())
		{
			Engines = Power->GetPower(EHDSubsystem::Engines);
		}
	}
	return BaseMaxSpeed * (0.5f + Engines / 100.f);
}

FHDShipState UHDShipMovementComponent::CaptureState(float Timestamp) const
{
	FHDShipState S;
	S.Timestamp = Timestamp;
	if (const AActor* Owner = GetOwner())
	{
		S.Location = Owner->GetActorLocation();
		S.Rotation = Owner->GetActorQuat();
	}
	S.Velocity = Velocity;
	S.AngularVelocity = AngularVelocity;
	return S;
}

void UHDShipMovementComponent::SimulateMove(const FHDShipMove& Move)
{
	AActor* Owner = GetOwner();
	const float Dt = Move.DeltaTime;
	if (!Owner || Dt <= 0.f) return;

	const bool bSlip = TravelMode == EHDTravelMode::Slipstream;
	const FVector RotIn = Move.Rot.BoundToCube(1.f) * (bSlip ? SlipTurnScale : 1.f);
	const FVector LinIn = bSlip ? FVector::ZeroVector : Move.Linear.BoundToCube(1.f);

	// Angular (deg/s in local pitch/yaw/roll)
	const FVector TargetAng = RotIn * MaxAngularRate;
	if (Move.bFlightAssist || bSlip)
	{
		AngularVelocity = FMath::VInterpConstantTo(AngularVelocity, TargetAng, Dt, AngularAccel);
	}
	else
	{
		AngularVelocity += RotIn * AngularAccel * Dt;
		AngularVelocity = AngularVelocity.BoundToBox(-MaxAngularRate, MaxAngularRate);
	}
	const FQuat Delta = FQuat(FRotator(AngularVelocity.X * Dt, AngularVelocity.Y * Dt, AngularVelocity.Z * Dt));
	const FQuat NewQ = (Owner->GetActorQuat() * Delta).GetNormalized();

	// Linear
	if (bSlip)
	{
		Velocity = NewQ.GetForwardVector() * SlipSpeed;
	}
	else
	{
		const float Cap = GetSpeedCap();
		if (Move.bFlightAssist)
		{
			const FVector TargetVel = NewQ.RotateVector(LinIn * Cap);
			Velocity = FMath::VInterpConstantTo(Velocity, TargetVel, Dt, LinearAccel);
		}
		else
		{
			Velocity += NewQ.RotateVector(LinIn) * LinearAccel * Dt;
		}
		Velocity = Velocity.GetClampedToMaxSize(Cap);
	}

	FHitResult Hit;
	Owner->SetActorLocationAndRotation(Owner->GetActorLocation() + Velocity * Dt, NewQ, true, &Hit);
	if (Hit.bBlockingHit)
	{
		Velocity = FVector::VectorPlaneProject(Velocity, Hit.Normal) * 0.5f;
		if (bSlip && Owner->HasAuthority()) { DropOut(true); }
	}
}

void UHDShipMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return;

	if (Pawn->HasAuthority()) { UpdateTravelAuthority(DeltaTime); }

	if (Pawn->IsLocallyControlled())
	{
		FHDShipMove M;
		M.DeltaTime = FMath::Min(DeltaTime, MaxMoveDeltaTime);
		ClientTime += M.DeltaTime;
		M.Timestamp = ClientTime;
		M.Linear = InputLinear.BoundToCube(1.f);

		// Rate inputs (keys, stick) are already -1..1 of max turn rate. Mouse is a delta in degrees:
		// convert to the rate that covers it in this tick, normalized by max rate. No double scaling.
		FVector Rot = InputRot;
		Rot.X += StickRot.X;
		Rot.Y += StickRot.Y;
		if (DeltaTime > KINDA_SMALL_NUMBER && !PendingMouseDegrees.IsZero())
		{
			if (MaxAngularRate.X > KINDA_SMALL_NUMBER) { Rot.X += PendingMouseDegrees.X / (DeltaTime * MaxAngularRate.X); }
			if (MaxAngularRate.Y > KINDA_SMALL_NUMBER) { Rot.Y += PendingMouseDegrees.Y / (DeltaTime * MaxAngularRate.Y); }
		}
		PendingMouseDegrees = FVector2D::ZeroVector;
		M.Rot = Rot.BoundToCube(1.f);
		M.bFlightAssist = bFlightAssist;
		SimulateMove(M);
		if (Pawn->HasAuthority())
		{
			ServerState = CaptureState(M.Timestamp);
		}
		else
		{
			Unacked.Add(M);
			if (Unacked.Num() > FMath::Max(1, MaxUnackedMoves)) { Unacked.RemoveAt(0); }
			ServerMove(M);
		}
	}
	else if (Pawn->HasAuthority())
	{
		// Remote-controlled ship on the server: grant real time the client may spend on moves.
		ServerTimeBudget = FMath::Min(ServerTimeBudget + DeltaTime, MaxClientTimeAhead);
	}
	else if (ServerState.Timestamp > 0.f)
	{
		// Simulated proxy: smooth toward server state (exponential, frame-rate independent).
		Velocity = ServerState.Velocity;
		AngularVelocity = ServerState.AngularVelocity;
		const FVector L = Pawn->GetActorLocation();
		if (FVector::DistSquared(L, ServerState.Location) > FMath::Square(ProxySnapDistance))
		{
			Pawn->SetActorLocationAndRotation(ServerState.Location, ServerState.Rotation);
		}
		else
		{
			const float Alpha = 1.f - FMath::Exp(-ProxySmoothingRate * DeltaTime);
			const FVector NewL = L + (ServerState.Location - L) * Alpha;
			const FQuat NewQ = FQuat::Slerp(Pawn->GetActorQuat(), ServerState.Rotation, Alpha);
			Pawn->SetActorLocationAndRotation(NewL, NewQ);
		}
	}
}

void UHDShipMovementComponent::ServerMove_Implementation(const FHDShipMove& Move)
{
	if (Move.Timestamp <= LastServerTimestamp) return; // stale/out of order
	if (!FMath::IsFinite(Move.Timestamp) || !FMath::IsFinite(Move.DeltaTime) || Move.Linear.ContainsNaN() || Move.Rot.ContainsNaN()) return;
	LastServerTimestamp = Move.Timestamp;
	FHDShipMove M = Move;
	// Clamp to max step and to the real time this client has been granted (stops speed hacks via fast clocks).
	M.DeltaTime = FMath::Clamp(M.DeltaTime, 0.f, FMath::Min(MaxMoveDeltaTime, FMath::Max(ServerTimeBudget, 0.f)));
	ServerTimeBudget -= M.DeltaTime;
	SimulateMove(M);
	ServerState = CaptureState(M.Timestamp);
	ClientAck(ServerState);
}

void UHDShipMovementComponent::ClientAck_Implementation(const FHDShipState& State)
{
	if (State.Timestamp <= LastAckTimestamp) return;
	LastAckTimestamp = State.Timestamp;
	Unacked.RemoveAll([&State](const FHDShipMove& M) { return M.Timestamp <= State.Timestamp; });

	AActor* Owner = GetOwner();
	if (!Owner) return;
	Owner->SetActorLocationAndRotation(State.Location, State.Rotation, false, nullptr, ETeleportType::TeleportPhysics);
	Velocity = State.Velocity;
	AngularVelocity = State.AngularVelocity;
	for (const FHDShipMove& M : Unacked) { SimulateMove(M); }
}

void UHDShipMovementComponent::ServerToggleSlipstream_Implementation()
{
	if (TravelMode != EHDTravelMode::Normal) { DropOut(false); return; }
	if (SlipCooldown > 0.f) return;
	TravelMode = EHDTravelMode::Spooling;
	SpoolRemaining = SpoolTime;
}

void UHDShipMovementComponent::DropOut(bool bInterdicted)
{
	if (TravelMode == EHDTravelMode::Normal) return;
	if (TravelMode == EHDTravelMode::Slipstream)
	{
		Velocity = Velocity.GetSafeNormal() * BaseMaxSpeed * DropOutSpeedFraction;
	}
	TravelMode = EHDTravelMode::Normal;
	SpoolRemaining = 0.f;
	TravelTarget.Reset();
	if (bInterdicted) { SlipCooldown = InterdictCooldown; }
}

AActor* UHDShipMovementComponent::PickSlipTarget() const
{
	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();
	if (!Owner || !World) return nullptr;
	AActor* Best = nullptr;
	double BestDist = TNumericLimits<double>::Max();
	const FVector Fwd = Owner->GetActorForwardVector();
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* A = *It;
		if (A == Owner || !A->ActorHasTag(TEXT("SlipTarget"))) continue;
		const FVector To = A->GetActorLocation() - Owner->GetActorLocation();
		const double Dist = To.Size();
		if (Dist < DropDistance * 2.f) continue;
		if (FVector::DotProduct(To / Dist, Fwd) < SlipTargetMinDot) continue;
		if (Dist < BestDist) { BestDist = Dist; Best = A; }
	}
	return Best;
}

void UHDShipMovementComponent::UpdateTravelAuthority(float DeltaTime)
{
	SlipCooldown = FMath::Max(0.f, SlipCooldown - DeltaTime);
	if (TravelMode == EHDTravelMode::Spooling)
	{
		SpoolRemaining -= DeltaTime;
		if (SpoolRemaining <= 0.f)
		{
			TravelMode = EHDTravelMode::Slipstream;
			TravelTarget = PickSlipTarget();
		}
	}
	else if (TravelMode == EHDTravelMode::Slipstream && TravelTarget.IsValid() && GetOwner())
	{
		if (FVector::Dist(TravelTarget->GetActorLocation(), GetOwner()->GetActorLocation()) < DropDistance + SlipSpeed * DeltaTime * 2.f)
		{
			DropOut(false);
		}
	}
}
