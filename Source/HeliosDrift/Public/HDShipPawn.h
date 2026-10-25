#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HDShipPawn.generated.h"

class UStaticMeshComponent;
class UHDShipPowerComponent;
class UHDShieldComponent;
class USceneComponent;
class UCameraComponent;

/** Player ship. Flight model (6DoF + flight assist) lands in M1 as a custom movement component. */
UCLASS()
class HELIOSDRIFT_API AHDShipPawn : public APawn
{
	GENERATED_BODY()
public:
	AHDShipPawn();
	virtual float TakeDamage(float Damage, struct FDamageEvent const& Event, AController* Instigator, AActor* Causer) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship") TObjectPtr<UStaticMeshComponent> Hull;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship") TObjectPtr<UHDShipPowerComponent> Power;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ship") TObjectPtr<UHDShieldComponent> Shields;
	UPROPERTY(EditDefaultsOnly, Replicated, Category = "Ship") float HullHP = 1000.f;

	/** Pilot eye anchor in the cockpit; HMD pose is applied relative to this. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cockpit") TObjectPtr<USceneComponent> CockpitRoot;
	/** Locked to HMD in VR; fixed at the eye anchor on flat screens. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cockpit") TObjectPtr<UCameraComponent> CockpitCamera;

	/** Reset HMD orientation/position to the seat. No-op without an HMD. */
	UFUNCTION(BlueprintCallable, Category = "VR") void RecenterVR();
	UFUNCTION(BlueprintPure, Category = "VR") static bool IsVRActive();

	virtual void PossessedBy(AController* NewController) override;
	virtual void NotifyControllerChanged() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
