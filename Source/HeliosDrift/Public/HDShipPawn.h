#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HDShipPawn.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UHDShipPowerComponent;
class UHDShieldComponent;
class UHDShipMovementComponent;
class UHDLaserComponent;
class USceneComponent;
class UCameraComponent;
struct FInputActionValue;

/** Player ship: 6DoF movement, power, 4-facing shields, laser, cockpit camera (flat + VR). */
UCLASS()
class HELIOSDRIFT_API AHDShipPawn : public APawn
{
	GENERATED_BODY()
public:
	AHDShipPawn();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& Event, AController* Instigator, AActor* Causer) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual FVector GetVelocity() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship") TObjectPtr<UBoxComponent> Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship") TObjectPtr<UStaticMeshComponent> Hull;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship") TObjectPtr<UHDShipPowerComponent> Power;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship") TObjectPtr<UHDShieldComponent> Shields;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship") TObjectPtr<UHDShipMovementComponent> Movement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship") TObjectPtr<UHDLaserComponent> Laser;
	UPROPERTY(EditDefaultsOnly, Replicated, BlueprintReadOnly, Category = "Ship") float HullHP = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Ship") float MaxHullHP = 1000.f;

	/** Pilot eye anchor in the cockpit; HMD pose is applied relative to this. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cockpit") TObjectPtr<USceneComponent> CockpitRoot;
	/** Locked to HMD in VR; fixed at the eye anchor on flat screens. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cockpit") TObjectPtr<UCameraComponent> CockpitCamera;

	/** Reset HMD orientation/position to the seat. No-op without an HMD. */
	UFUNCTION(BlueprintCallable, Category = "VR") void RecenterVR();
	UFUNCTION(BlueprintPure, Category = "VR") static bool IsVRActive();

	virtual void NotifyControllerChanged() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void OnThrust(const FInputActionValue& V);
	void OnStrafe(const FInputActionValue& V);
	void OnVertical(const FInputActionValue& V);
	void OnPitch(const FInputActionValue& V);
	void OnYaw(const FInputActionValue& V);
	void OnRoll(const FInputActionValue& V);
	void OnFireStart(const FInputActionValue& V);
	void OnFireStop(const FInputActionValue& V);
	void OnToggleFA(const FInputActionValue& V);
	void OnSlipstream(const FInputActionValue& V);
	void OnPowerBalanced(const FInputActionValue& V);
	void OnPowerAttack(const FInputActionValue& V);
	void OnPowerTank(const FInputActionValue& V);
	void OnPowerRun(const FInputActionValue& V);
	void OnEqualize(const FInputActionValue& V);
	void OnRecenter(const FInputActionValue& V);
	void SetPower(float W, float S, float E, float A);
};
