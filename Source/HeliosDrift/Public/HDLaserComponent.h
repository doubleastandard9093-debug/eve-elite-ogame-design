#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/EngineTypes.h"
#include "HDLaserComponent.generated.h"

/** Hitscan pulse laser (doc 02). Server traces and applies damage; tracers are multicast debug lines. */
UCLASS(ClassGroup = (Ship), meta = (BlueprintSpawnableComponent))
class HELIOSDRIFT_API UHDLaserComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UHDLaserComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Weapon") void ServerSetFiring(bool bFiring);

	UPROPERTY(EditAnywhere, Category = "Weapon") float Range = 500000.f;     // 5 km
	UPROPERTY(EditAnywhere, Category = "Weapon") float Damage = 40.f;        // at 50 weapons power
	UPROPERTY(EditAnywhere, Category = "Weapon") float FireInterval = 0.15f;
	UPROPERTY(EditAnywhere, Category = "Weapon") float MuzzleOffset = 500.f;
	/** Damage multiplier = PowerScaleBase + WeaponsPower / PowerScaleDivisor (1.0 at 50 power). */
	UPROPERTY(EditAnywhere, Category = "Weapon") float PowerScaleBase = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Weapon", meta = (ClampMin = "1")) float PowerScaleDivisor = 100.f;
	UPROPERTY(EditAnywhere, Category = "Weapon") TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

	/** Server: stop firing immediately (death, slipstream, etc.). */
	void StopFiringAuthority() { bWantsFire = false; }

protected:
	UFUNCTION(NetMulticast, Unreliable) void MulticastTracer(const FVector& Start, const FVector& End, bool bHit);

private:
	void FireOnce();
	bool bWantsFire = false;
	float Cooldown = 0.f;
};
