#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HDPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/** Runtime-built Enhanced Input actions (no .uasset needed). */
USTRUCT()
struct FHDFlightInput
{
	GENERATED_BODY()
	UPROPERTY(Transient) TObjectPtr<UInputMappingContext> Context;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Thrust;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Strafe;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Vertical;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Pitch;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Yaw;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Roll;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Fire;
	UPROPERTY(Transient) TObjectPtr<UInputAction> ToggleFA;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Slipstream;
	UPROPERTY(Transient) TObjectPtr<UInputAction> PowerBalanced;
	UPROPERTY(Transient) TObjectPtr<UInputAction> PowerAttack;
	UPROPERTY(Transient) TObjectPtr<UInputAction> PowerTank;
	UPROPERTY(Transient) TObjectPtr<UInputAction> PowerRun;
	UPROPERTY(Transient) TObjectPtr<UInputAction> EqualizeShields;
	UPROPERTY(Transient) TObjectPtr<UInputAction> RecenterVR;
};

UCLASS()
class HELIOSDRIFT_API AHDPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	/** Builds actions + mapping context on first call (local controller only) and registers the context. */
	const FHDFlightInput& GetFlightInput();

protected:
	virtual void BeginPlay() override;

	/** Optional: assign an asset-based context in a BP subclass to add on top of / replace the runtime one. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> FlightMapping;

	UPROPERTY(Transient) FHDFlightInput Flight;

private:
	void BuildFlightInput();
	bool bFlightBuilt = false;
};
