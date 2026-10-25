#include "HDShipPowerComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"

static constexpr int32 NumSub = (int32)EHDSubsystem::MAX;
static constexpr int32 NumFace = (int32)EHDShieldFacing::MAX;

UHDShipPowerComponent::UHDShipPowerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	Current.Init(50.f, NumSub);
	Target.Init(50.f, NumSub);
}

void UHDShipPowerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHDShipPowerComponent, Current);
}

void UHDShipPowerComponent::ServerSetAllocation_Implementation(const TArray<float>& NewTargets)
{
	if (NewTargets.Num() != NumSub) return;
	float Sum = 0.f;
	for (float V : NewTargets) Sum += FMath::Clamp(V, 0.f, 100.f);
	const float Scale = Sum > ReactorBudget ? ReactorBudget / Sum : 1.f;
	for (int32 i = 0; i < NumSub; ++i) Target[i] = FMath::Clamp(NewTargets[i], 0.f, 100.f) * Scale;
}

void UHDShipPowerComponent::TickComponent(float Dt, ELevelTick TT, FActorComponentTickFunction* F)
{
	Super::TickComponent(Dt, TT, F);
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	for (int32 i = 0; i < NumSub; ++i) Current[i] = FMath::FInterpConstantTo(Current[i], Target[i], Dt, RampPerSecond);
}

UHDShieldComponent::UHDShieldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	Facings.Init(MaxPerFacing, NumFace);
}

void UHDShieldComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHDShieldComponent, Facings);
}

EHDShieldFacing UHDShieldComponent::FacingFromLocalDir(const FVector& D)
{
	if (FMath::Abs(D.X) >= FMath::Abs(D.Y)) return D.X >= 0 ? EHDShieldFacing::Fore : EHDShieldFacing::Aft;
	return D.Y >= 0 ? EHDShieldFacing::Starboard : EHDShieldFacing::Port;
}

float UHDShieldComponent::AbsorbDamage(float Amount, const FVector& WorldHitDir)
{
	const FVector Local = GetOwner()->GetActorTransform().InverseTransformVectorNoScale(-WorldHitDir);
	float& F = Facings[(int32)FacingFromLocalDir(Local)];
	const float Absorbed = FMath::Min(F, Amount);
	F -= Absorbed;
	return Amount - Absorbed;
}

void UHDShieldComponent::ServerEqualize_Implementation()
{
	float Sum = 0.f;
	for (float V : Facings) Sum += V;
	for (float& V : Facings) V = Sum / NumFace;
}

void UHDShieldComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner() && GetOwner()->HasAuthority()) { Refill(); }
}

void UHDShieldComponent::Refill()
{
	Facings.Init(MaxPerFacing, NumFace);
}

void UHDShieldComponent::TickComponent(float Dt, ELevelTick TT, FActorComponentTickFunction* F)
{
	Super::TickComponent(Dt, TT, F);
	AActor* Owner = GetOwner();
	if (!Owner || !Owner->HasAuthority()) return;
	float Scale = 1.f;
	if (const UHDShipPowerComponent* Power = Owner->FindComponentByClass<UHDShipPowerComponent>())
	{
		Scale = 0.5f + Power->GetPower(EHDSubsystem::Shields) / 100.f;
	}
	for (float& V : Facings) { V = FMath::Min(MaxPerFacing, V + RegenPerSecond * Scale * Dt); }
}
