#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HDShipPowerComponent.generated.h"

UENUM(BlueprintType)
enum class EHDSubsystem : uint8 { Weapons, Shields, Engines, Auxiliary, MAX UMETA(Hidden) };

UENUM(BlueprintType)
enum class EHDShieldFacing : uint8 { Fore, Aft, Port, Starboard, MAX UMETA(Hidden) };

/** Reactor power allocation (doc 02). Targets set by client RPC, current values ramp on server and replicate. */
UCLASS(ClassGroup = (Ship), meta = (BlueprintSpawnableComponent))
class HELIOSDRIFT_API UHDShipPowerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UHDShipPowerComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Power")
	void ServerSetAllocation(const TArray<float>& NewTargets);

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPower(EHDSubsystem S) const { return Current.IsValidIndex((int32)S) ? Current[(int32)S] : 0.f; }

	UPROPERTY(EditDefaultsOnly, Category = "Power") float ReactorBudget = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "Power") float RampPerSecond = 100.f;

protected:
	UPROPERTY(Replicated) TArray<float> Current;
	UPROPERTY() TArray<float> Target;
};

/** Four-facing shields (doc 02). */
UCLASS(ClassGroup = (Ship), meta = (BlueprintSpawnableComponent))
class HELIOSDRIFT_API UHDShieldComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UHDShieldComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Server: absorb damage on the facing toward WorldHitDir; returns overflow for hull. */
	float AbsorbDamage(float Amount, const FVector& WorldHitDir);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Shields")
	void ServerEqualize();

	static EHDShieldFacing FacingFromLocalDir(const FVector& LocalDir);

	UPROPERTY(EditDefaultsOnly, Category = "Shields") float MaxPerFacing = 500.f;
	/** Per-facing regen per second at 50 shield power (scales with Shields power if owner has a power component). */
	UPROPERTY(EditDefaultsOnly, Category = "Shields") float RegenPerSecond = 20.f;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Server: restore all facings to max. */
	void Refill();

	UFUNCTION(BlueprintPure, Category = "Shields")
	float GetFacing(EHDShieldFacing F) const { return Facings.IsValidIndex((int32)F) ? Facings[(int32)F] : 0.f; }

protected:
	UPROPERTY(Replicated) TArray<float> Facings;
};
