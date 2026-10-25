#include "HDShipPawn.h"
#include "HDShipPowerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

AHDShipPawn::AHDShipPawn()
{
	bReplicates = true;
	SetReplicateMovement(true);
	Hull = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hull"));
	RootComponent = Hull;
	Power = CreateDefaultSubobject<UHDShipPowerComponent>(TEXT("Power"));
	Shields = CreateDefaultSubobject<UHDShieldComponent>(TEXT("Shields"));
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
