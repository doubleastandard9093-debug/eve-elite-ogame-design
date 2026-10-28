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
	/** Rate axes (-1..1): keyboard, triggers, left stick. */
	UPROPERTY(Transient) TObjectPtr<UInputAction> Thrust;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Strafe;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Vertical;
	/** Rate axes (-1..1): arrow keys only. */
	UPROPERTY(Transient) TObjectPtr<UInputAction> Pitch;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Yaw;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Roll;
	/** Right stick (Axis2D, rate-based: X = yaw, Y = pitch). */
	UPROPERTY(Transient) TObjectPtr<UInputAction> LookStick;
	/** Mouse (Axis2D, delta-based: counts moved this frame). */
	UPROPERTY(Transient) TObjectPtr<UInputAction> MouseAim;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Fire;
	UPROPERTY(Transient) TObjectPtr<UInputAction> ToggleFA;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Slipstream;
	UPROPERTY(Transient) TObjectPtr<UInputAction> PowerBalanced;
	UPROPERTY(Transient) TObjectPtr<UInputAction> PowerAttack;
	UPROPERTY(Transient) TObjectPtr<UInputAction> PowerTank;
	UPROPERTY(Transient) TObjectPtr<UInputAction> PowerRun;
	UPROPERTY(Transient) TObjectPtr<UInputAction> EqualizeShields;
	UPROPERTY(Transient) TObjectPtr<UInputAction> RecenterVR;
	UPROPERTY(Transient) TObjectPtr<UInputAction> Menu;
};

UCLASS(Config = Game)
class HELIOSDRIFT_API AHDPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	/** Builds actions + mapping context on first call (local controller only) and registers the context. */
	const FHDFlightInput& GetFlightInput();

	/** Multiplier on right-stick look rate (1 = full stick gives the ship's max turn rate). Settable in DefaultGame.ini or with the console. */
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Input|Gamepad", meta = (ClampMin = "0.1", ClampMax = "3.0"))
	float StickSensitivity = 1.0f;

	/** Response-curve exponent for both sticks (1 = linear, 2 = fine control near centre). Applied when the context is built. */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Gamepad", meta = (ClampMin = "1.0", ClampMax = "4.0"))
	float StickCurveExponent = 2.0f;

	/** Inner deadzone for sticks and triggers (0..1). Applied when the context is built. */
	UPROPERTY(EditAnywhere, Config, Category = "Input|Gamepad", meta = (ClampMin = "0.0", ClampMax = "0.5"))
	float StickDeadZone = 0.15f;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Input|Gamepad")
	bool bInvertStickPitch = false;

	/** Degrees of ship rotation per mouse count (delta-based; independent of frame rate). */
	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Input|Mouse", meta = (ClampMin = "0.001", ClampMax = "2.0"))
	float MouseDegreesPerCount = 0.15f;

	UPROPERTY(EditAnywhere, Config, BlueprintReadWrite, Category = "Input|Mouse")
	bool bInvertMousePitch = false;

	/** Console: HDStickSensitivity 1.5 */
	UFUNCTION(Exec) void HDStickSensitivity(float Value);
	/** Console: HDMouseSensitivity 0.15 (degrees per count) */
	UFUNCTION(Exec) void HDMouseSensitivity(float Value);

	/** Toggled by Menu / Esc. M1 shows the controls overlay in the HUD; a real menu comes later. */
	UFUNCTION(BlueprintCallable, Category = "UI") void ToggleMenu() { bMenuOpen = !bMenuOpen; }
	UFUNCTION(BlueprintPure, Category = "UI") bool IsMenuOpen() const { return bMenuOpen; }

protected:
	virtual void BeginPlay() override;

	/** Optional: assign an asset-based context in a BP subclass to add on top of / replace the runtime one. */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> FlightMapping;

	UPROPERTY(Transient) FHDFlightInput Flight;

private:
	void BuildFlightInput();
	bool bFlightBuilt = false;
	bool bMenuOpen = false;
};
