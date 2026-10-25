#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HDShipPawn.generated.h"

class UStaticMeshComponent;
class UHDShipPowerComponent;
class UHDShieldComponent;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
