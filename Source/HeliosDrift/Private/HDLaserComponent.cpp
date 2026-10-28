#include "HDLaserComponent.h"
#include "HDShipPowerComponent.h"
#include "HDShipMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"

UHDLaserComponent::UHDLaserComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UHDLaserComponent::ServerSetFiring_Implementation(bool bFiring)
{
	bWantsFire = bFiring;
}

void UHDLaserComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	Cooldown -= DeltaTime;
	if (!bWantsFire)
	{
		Cooldown = FMath::Max(0.f, Cooldown);
		return;
	}
	// Keep the remainder so the fire rate does not depend on frame rate; cap shots per tick.
	const float Interval = FMath::Max(FireInterval, 0.02f);
	int32 Shots = 0;
	while (Cooldown <= 0.f && Shots < 4)
	{
		FireOnce();
		Cooldown += Interval;
		++Shots;
	}
	if (Cooldown < 0.f) { Cooldown = 0.f; } // hitched frame: drop excess shots instead of bursting later
}

void UHDLaserComponent::FireOnce()
{
	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();
	if (!Owner || !World) return;
	// Server validation: no weapons while spooling or in slipstream.
	if (const UHDShipMovementComponent* Move = Owner->FindComponentByClass<UHDShipMovementComponent>())
	{
		if (Move->GetTravelMode() != EHDTravelMode::Normal) return;
	}

	const FVector Fwd = Owner->GetActorForwardVector();
	const FVector Start = Owner->GetActorLocation() + Fwd * MuzzleOffset;
	const FVector End = Start + Fwd * Range;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(HDLaser), false, Owner);
	FHitResult Hit;
	const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, TraceChannel.GetValue(), Params);

	if (bHit && Hit.GetActor())
	{
		float Weapons = 50.f;
		if (const UHDShipPowerComponent* Power = Owner->FindComponentByClass<UHDShipPowerComponent>())
		{
			Weapons = Power->GetPower(EHDSubsystem::Weapons);
		}
		const float Dmg = FMath::Max(0.f, Damage * (PowerScaleBase + Weapons / FMath::Max(PowerScaleDivisor, 1.f)));
		const APawn* Pawn = Cast<APawn>(Owner);
		UGameplayStatics::ApplyDamage(Hit.GetActor(), Dmg, Pawn ? Pawn->GetController() : nullptr, Owner, UDamageType::StaticClass());
	}
	MulticastTracer(Start, bHit ? Hit.ImpactPoint : End, bHit);
}

void UHDLaserComponent::MulticastTracer_Implementation(const FVector& Start, const FVector& End, bool bHit)
{
#if ENABLE_DRAW_DEBUG
	if (UWorld* World = GetWorld())
	{
		DrawDebugLine(World, Start, End, bHit ? FColor::Orange : FColor::Red, false, 0.08f, 0, 12.f);
		if (bHit) { DrawDebugSphere(World, End, 80.f, 8, FColor::Yellow, false, 0.15f); }
	}
#endif
}
