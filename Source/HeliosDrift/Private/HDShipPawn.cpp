#include "HDShipPawn.h"
#include "HDShipPowerComponent.h"
#include "HDShipMovementComponent.h"
#include "HDLaserComponent.h"
#include "HDPlayerController.h"
#include "HeliosDrift.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/World.h"
#include "HeadMountedDisplayFunctionLibrary.h" // XRBase plugin (moved from HeadMountedDisplay in UE 5.3+)

AHDShipPawn::AHDShipPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(false); // UHDShipMovementComponent replicates its own state
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Collision->SetBoxExtent(FVector(400.f, 200.f, 100.f));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetCollisionObjectType(ECC_Pawn);
	Collision->SetCollisionResponseToAllChannels(ECR_Block);
	RootComponent = Collision;

	Hull = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hull"));
	Hull->SetupAttachment(Collision);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded()) { Hull->SetStaticMesh(Cube.Object); }
	Hull->SetRelativeScale3D(FVector(8.f, 4.f, 2.f)); // engine cube is 100cm
	Hull->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Hull->SetOwnerNoSee(true); // cockpit view: don't render own hull box

	Power = CreateDefaultSubobject<UHDShipPowerComponent>(TEXT("Power"));
	Shields = CreateDefaultSubobject<UHDShieldComponent>(TEXT("Shields"));
	Movement = CreateDefaultSubobject<UHDShipMovementComponent>(TEXT("Movement"));
	Laser = CreateDefaultSubobject<UHDLaserComponent>(TEXT("Laser"));

	CockpitRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CockpitRoot"));
	CockpitRoot->SetupAttachment(Collision);
	CockpitRoot->SetRelativeLocation(FVector(300.f, 0.f, 80.f)); // tune per hull in BP
	CockpitCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CockpitCamera"));
	CockpitCamera->SetupAttachment(CockpitRoot);
	CockpitCamera->bUsePawnControlRotation = false;
	CockpitCamera->bLockToHmd = true; // ignored when no HMD is active
}

void AHDShipPawn::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) { HullHP = MaxHullHP; }
}

FVector AHDShipPawn::GetVelocity() const
{
	return Movement ? Movement->GetShipVelocity() : FVector::ZeroVector;
}

bool AHDShipPawn::IsVRActive()
{
	return UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
}

void AHDShipPawn::RecenterVR()
{
	if (IsVRActive())
	{
		UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(0.f, EOrientPositionSelector::OrientationAndPosition);
	}
}

void AHDShipPawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	if (IsLocallyControlled()) { RecenterVR(); }
}

void AHDShipPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHDShipPawn, HullHP);
}

float AHDShipPawn::TakeDamage(float Damage, FDamageEvent const& Event, AController* Inst, AActor* Causer)
{
	if (!HasAuthority() || HullHP <= 0.f || !(Damage > 0.f) || IsPendingKillPending()) return 0.f;
	FVector Dir = Causer ? (GetActorLocation() - Causer->GetActorLocation()).GetSafeNormal() : FVector::ZeroVector;
	if (Dir.IsNearlyZero()) { Dir = -GetActorForwardVector(); } // unknown source: treat as a hit on the fore facing
	const float Overflow = Shields ? Shields->AbsorbDamage(Damage, Dir) : Damage;
	HullHP = FMath::Max(0.f, HullHP - Overflow);
	if (Movement) { Movement->DropOut(true); } // any hit interdicts slipstream / spool

	if (HullHP <= 0.f)
	{
		UE_LOG(LogHelios, Log, TEXT("Ship %s destroyed"), *GetName());
		// TODO(M2): spawn wreck, report loss to backend.
		AController* C = GetController();
		UWorld* World = GetWorld();
		if (Laser) { Laser->StopFiringAuthority(); }
		if (C) { C->UnPossess(); }
		Destroy();
		if (C && World)
		{
			if (AGameModeBase* GM = World->GetAuthGameMode()) { GM->RestartPlayer(C); }
		}
	}
	return Damage;
}

void AHDShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AHDPlayerController* PC = Cast<AHDPlayerController>(GetController());
	if (!EIC || !PC)
	{
		UE_LOG(LogHelios, Warning, TEXT("Ship input not bound (EnhancedInputComponent=%d, HDPlayerController=%d)"), EIC != nullptr, PC != nullptr);
		return;
	}
	const FHDFlightInput& In = PC->GetFlightInput();

	auto BindAxis = [this, EIC](UInputAction* A, void (AHDShipPawn::*Fn)(const FInputActionValue&))
	{
		if (!A) return;
		EIC->BindAction(A, ETriggerEvent::Triggered, this, Fn);
		EIC->BindAction(A, ETriggerEvent::Completed, this, Fn);
	};
	auto BindPress = [this, EIC](UInputAction* A, void (AHDShipPawn::*Fn)(const FInputActionValue&))
	{
		if (A) { EIC->BindAction(A, ETriggerEvent::Started, this, Fn); }
	};

	BindAxis(In.Thrust, &AHDShipPawn::OnThrust);
	BindAxis(In.Strafe, &AHDShipPawn::OnStrafe);
	BindAxis(In.Vertical, &AHDShipPawn::OnVertical);
	BindAxis(In.Pitch, &AHDShipPawn::OnPitch);
	BindAxis(In.Yaw, &AHDShipPawn::OnYaw);
	BindAxis(In.Roll, &AHDShipPawn::OnRoll);
	if (In.LookStick)
	{
		EIC->BindAction(In.LookStick, ETriggerEvent::Triggered, this, &AHDShipPawn::OnLookStick);
		EIC->BindAction(In.LookStick, ETriggerEvent::Completed, this, &AHDShipPawn::OnLookStick);
	}
	if (In.MouseAim) { EIC->BindAction(In.MouseAim, ETriggerEvent::Triggered, this, &AHDShipPawn::OnMouseAim); }
	BindPress(In.Menu, &AHDShipPawn::OnMenu);
	BindPress(In.Fire, &AHDShipPawn::OnFireStart);
	if (In.Fire) { EIC->BindAction(In.Fire, ETriggerEvent::Completed, this, &AHDShipPawn::OnFireStop); }
	BindPress(In.ToggleFA, &AHDShipPawn::OnToggleFA);
	BindPress(In.Slipstream, &AHDShipPawn::OnSlipstream);
	BindPress(In.PowerBalanced, &AHDShipPawn::OnPowerBalanced);
	BindPress(In.PowerAttack, &AHDShipPawn::OnPowerAttack);
	BindPress(In.PowerTank, &AHDShipPawn::OnPowerTank);
	BindPress(In.PowerRun, &AHDShipPawn::OnPowerRun);
	BindPress(In.EqualizeShields, &AHDShipPawn::OnEqualize);
	BindPress(In.RecenterVR, &AHDShipPawn::OnRecenter);
}

void AHDShipPawn::OnThrust(const FInputActionValue& V)   { if (Movement) Movement->InputLinear.X = V.Get<float>(); }
void AHDShipPawn::OnStrafe(const FInputActionValue& V)   { if (Movement) Movement->InputLinear.Y = V.Get<float>(); }
void AHDShipPawn::OnVertical(const FInputActionValue& V) { if (Movement) Movement->InputLinear.Z = V.Get<float>(); }
void AHDShipPawn::OnPitch(const FInputActionValue& V)    { if (Movement) Movement->InputRot.X = V.Get<float>(); }
void AHDShipPawn::OnYaw(const FInputActionValue& V)      { if (Movement) Movement->InputRot.Y = V.Get<float>(); }
void AHDShipPawn::OnRoll(const FInputActionValue& V)     { if (Movement) Movement->InputRot.Z = V.Get<float>(); }

void AHDShipPawn::OnLookStick(const FInputActionValue& V)
{
	if (!Movement) return;
	const FVector2D Stick = V.Get<FVector2D>(); // X right, Y up; deadzone + curve already applied by modifiers
	float Sens = 1.f;
	bool bInvert = false;
	if (const AHDPlayerController* PC = Cast<AHDPlayerController>(GetController()))
	{
		Sens = FMath::Clamp(PC->StickSensitivity, 0.1f, 3.f);
		bInvert = PC->bInvertStickPitch;
	}
	// Rate-based: deflection maps to a fraction of max turn rate. Not multiplied by DeltaTime here.
	Movement->StickRot = FVector2D(Stick.Y * (bInvert ? -1.f : 1.f), Stick.X) * Sens;
}

void AHDShipPawn::OnMouseAim(const FInputActionValue& V)
{
	if (!Movement) return;
	const FVector2D Counts = V.Get<FVector2D>(); // counts moved this frame (delta-based)
	float DegPerCount = 0.15f;
	bool bInvert = false;
	if (const AHDPlayerController* PC = Cast<AHDPlayerController>(GetController()))
	{
		DegPerCount = PC->MouseDegreesPerCount;
		bInvert = PC->bInvertMousePitch;
	}
	Movement->AddMouseAimDegrees(FVector2D(Counts.Y * (bInvert ? -1.f : 1.f), Counts.X) * DegPerCount);
}

void AHDShipPawn::OnMenu(const FInputActionValue& V)
{
	if (AHDPlayerController* PC = Cast<AHDPlayerController>(GetController())) { PC->ToggleMenu(); }
}

void AHDShipPawn::OnFireStart(const FInputActionValue& V) { if (Laser) Laser->ServerSetFiring(true); }
void AHDShipPawn::OnFireStop(const FInputActionValue& V)  { if (Laser) Laser->ServerSetFiring(false); }
void AHDShipPawn::OnToggleFA(const FInputActionValue& V)  { if (Movement) Movement->ToggleFlightAssist(); }
void AHDShipPawn::OnSlipstream(const FInputActionValue& V) { if (Movement) Movement->ServerToggleSlipstream(); }
void AHDShipPawn::OnPowerBalanced(const FInputActionValue& V) { SetPower(PresetBalanced); }
void AHDShipPawn::OnPowerAttack(const FInputActionValue& V)   { SetPower(PresetAttack); }
void AHDShipPawn::OnPowerTank(const FInputActionValue& V)     { SetPower(PresetTank); }
void AHDShipPawn::OnPowerRun(const FInputActionValue& V)      { SetPower(PresetRun); }
void AHDShipPawn::OnEqualize(const FInputActionValue& V)  { if (Shields) Shields->ServerEqualize(); }
void AHDShipPawn::OnRecenter(const FInputActionValue& V)  { RecenterVR(); }

void AHDShipPawn::SetPower(const FVector4& WSEA)
{
	if (!Power) return;
	TArray<float> Alloc;
	Alloc.Add((float)WSEA.X); Alloc.Add((float)WSEA.Y); Alloc.Add((float)WSEA.Z); Alloc.Add((float)WSEA.W);
	Power->ServerSetAllocation(Alloc);
}
