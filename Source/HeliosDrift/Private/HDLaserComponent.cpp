#include "HDLaserComponent.h"
#include "HDShipPowerComponent.h"
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
	Cooldown = FMath::Max(0.f, Cooldown - DeltaTime);
	if (bWantsFire && Cooldown <= 0.f)
	{
		FireOnce();
		Cooldown = FireInterval;
	}
}

void UHDLaserComponent::FireOnce()
{
	AActor* Owner = GetOwner();
	UWorld* World = GetWorld();
	if (!Owner || !World) return;

	const FVector Fwd = Owner->GetActorForwardVector();
	const FVector Start = Owner->GetActorLocation() + Fwd * MuzzleOffset;
	const FVector End = Start + Fwd * Range;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(HDLaser), false, Owner);
	FHitResult Hit;
	const bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	if (bHit && Hit.GetActor())
	{
		float Weapons = 50.f;
		if (const UHDShipPowerComponent* Power = Owner->FindComponentByClass<UHDShipPowerComponent>())
		{
			Weapons = Power->GetPower(EHDSubsystem::Weapons);
		}
		const float Dmg = Damage * (0.5f + Weapons / 100.f);
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
