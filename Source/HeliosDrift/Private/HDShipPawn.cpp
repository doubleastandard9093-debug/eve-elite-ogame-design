#include "HDShipPawn.h"
#include "HDShipPowerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Camera/CameraComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"

AHDShipPawn::AHDShipPawn()
{
	bReplicates = true;
	SetReplicateMovement(true);
	Hull = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hull"));
	RootComponent = Hull;
	Power = CreateDefaultSubobject<UHDShipPowerComponent>(TEXT("Power"));
	Shields = CreateDefaultSubobject<UHDShieldComponent>(TEXT("Shields"));

	CockpitRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CockpitRoot"));
	CockpitRoot->SetupAttachment(Hull);
	CockpitRoot->SetRelativeLocation(FVector(150.f, 0.f, 60.f)); // tune per hull in BP
	CockpitCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CockpitCamera"));
	CockpitCamera->SetupAttachment(CockpitRoot);
	CockpitCamera->bLockToHmd = true; // ignored when no HMD is active
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

void AHDShipPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AHDShipPawn::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	if (IsLocallyControlled()) { RecenterVR(); }
}

void AHDShipPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& Out) const
{
	Super::GetLifetimeReplicatedProps(Out);
	DOREPLIFETIME(AHDShipPawn, HullHP);
}

float AHDShipPawn::TakeDamage(float Damage, FDamageEvent const& Event, AController* Inst, AActor* Causer)
{
	if (!HasAuthority()) return 0.f;
	const FVector Dir = Causer ? (GetActorLocation() - Causer->GetActorLocation()).GetSafeNormal() : GetActorForwardVector();
	const float Overflow = Shields->AbsorbDamage(Damage, Dir);
	HullHP = FMath::Max(0.f, HullHP - Overflow);
	// TODO(M2): on HullHP==0 spawn wreck, report loss to backend.
	return Damage;
}
