#include "HDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputModifiers.h"
#include "InputCoreTypes.h"
#include "Engine/LocalPlayer.h"

namespace
{
	UInputAction* MakeAction(UObject* Outer, const TCHAR* Name, EInputActionValueType Type)
	{
		UInputAction* A = NewObject<UInputAction>(Outer, FName(Name));
		A->ValueType = Type;
		return A;
	}

	FEnhancedActionKeyMapping& MapKeyTo(UInputMappingContext* Ctx, UInputAction* Action, const FKey& Key, float Scale = 1.f)
	{
		FEnhancedActionKeyMapping& M = Ctx->MapKey(Action, Key);
		if (!FMath::IsNearlyEqual(Scale, 1.f))
		{
			UInputModifierScalar* S = NewObject<UInputModifierScalar>(Ctx);
			S->Scalar = FVector(Scale, Scale, Scale);
			M.Modifiers.Add(S);
		}
		return M;
	}

	/** Analog key: deadzone first, then response curve, then optional sign/scale. */
	void MapAnalog(UInputMappingContext* Ctx, UInputAction* Action, const FKey& Key, float DeadZone, float CurveExp, EDeadZoneType DZType, FVector Scale = FVector::OneVector)
	{
		FEnhancedActionKeyMapping& M = Ctx->MapKey(Action, Key);
		UInputModifierDeadZone* DZ = NewObject<UInputModifierDeadZone>(Ctx);
		DZ->LowerThreshold = DeadZone;
		DZ->UpperThreshold = 1.f;
		DZ->Type = DZType;
		M.Modifiers.Add(DZ);
		if (!FMath::IsNearlyEqual(CurveExp, 1.f))
		{
			UInputModifierResponseCurveExponential* Curve = NewObject<UInputModifierResponseCurveExponential>(Ctx);
			Curve->CurveExponent = FVector(CurveExp, CurveExp, 1.f);
			M.Modifiers.Add(Curve);
		}
		if (!Scale.Equals(FVector::OneVector))
		{
			UInputModifierScalar* S = NewObject<UInputModifierScalar>(Ctx);
			S->Scalar = Scale;
			M.Modifiers.Add(S);
		}
	}
}

void AHDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController()) { GetFlightInput(); }
}

void AHDPlayerController::HDStickSensitivity(float Value)
{
	StickSensitivity = FMath::Clamp(Value, 0.1f, 3.f);
}

void AHDPlayerController::HDMouseSensitivity(float Value)
{
	MouseDegreesPerCount = FMath::Clamp(Value, 0.001f, 2.f);
}

const FHDFlightInput& AHDPlayerController::GetFlightInput()
{
	if (!bFlightBuilt && IsLocalController())
	{
		BuildFlightInput();
		if (UEnhancedInputLocalPlayerSubsystem* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Sub->AddMappingContext(Flight.Context, 0);
			if (FlightMapping) { Sub->AddMappingContext(FlightMapping, 1); }
			bFlightBuilt = true;
		}
	}
	return Flight;
}

void AHDPlayerController::BuildFlightInput()
{
	if (Flight.Context) return;
	using T = EInputActionValueType;
	UInputMappingContext* C = NewObject<UInputMappingContext>(this, TEXT("IMC_Flight_Runtime"));
	Flight.Context = C;

	Flight.Thrust    = MakeAction(this, TEXT("IA_Thrust"), T::Axis1D);
	Flight.Strafe    = MakeAction(this, TEXT("IA_Strafe"), T::Axis1D);
	Flight.Vertical  = MakeAction(this, TEXT("IA_Vertical"), T::Axis1D);
	Flight.Pitch     = MakeAction(this, TEXT("IA_Pitch"), T::Axis1D);
	Flight.Yaw       = MakeAction(this, TEXT("IA_Yaw"), T::Axis1D);
	Flight.Roll      = MakeAction(this, TEXT("IA_Roll"), T::Axis1D);
	Flight.LookStick = MakeAction(this, TEXT("IA_LookStick"), T::Axis2D);
	Flight.MouseAim  = MakeAction(this, TEXT("IA_MouseAim"), T::Axis2D);
	Flight.Fire          = MakeAction(this, TEXT("IA_Fire"), T::Boolean);
	Flight.ToggleFA      = MakeAction(this, TEXT("IA_ToggleFA"), T::Boolean);
	Flight.Slipstream    = MakeAction(this, TEXT("IA_Slipstream"), T::Boolean);
	Flight.PowerBalanced = MakeAction(this, TEXT("IA_PowerBalanced"), T::Boolean);
	Flight.PowerAttack   = MakeAction(this, TEXT("IA_PowerAttack"), T::Boolean);
	Flight.PowerTank     = MakeAction(this, TEXT("IA_PowerTank"), T::Boolean);
	Flight.PowerRun      = MakeAction(this, TEXT("IA_PowerRun"), T::Boolean);
	Flight.EqualizeShields = MakeAction(this, TEXT("IA_EqualizeShields"), T::Boolean);
	Flight.RecenterVR    = MakeAction(this, TEXT("IA_RecenterVR"), T::Boolean);
	Flight.Menu          = MakeAction(this, TEXT("IA_Menu"), T::Boolean);

	// Keyboard + mouse
	MapKeyTo(C, Flight.Thrust, EKeys::W);            MapKeyTo(C, Flight.Thrust, EKeys::S, -1.f);
	MapKeyTo(C, Flight.Strafe, EKeys::D);            MapKeyTo(C, Flight.Strafe, EKeys::A, -1.f);
	MapKeyTo(C, Flight.Vertical, EKeys::R);          MapKeyTo(C, Flight.Vertical, EKeys::F, -1.f);
	MapKeyTo(C, Flight.Roll, EKeys::E);              MapKeyTo(C, Flight.Roll, EKeys::Q, -1.f);
	MapKeyTo(C, Flight.MouseAim, EKeys::Mouse2D);    // raw counts; converted to rotation in the movement component
	MapKeyTo(C, Flight.Pitch, EKeys::Up);            MapKeyTo(C, Flight.Pitch, EKeys::Down, -1.f);
	MapKeyTo(C, Flight.Yaw, EKeys::Right);           MapKeyTo(C, Flight.Yaw, EKeys::Left, -1.f);
	MapKeyTo(C, Flight.Fire, EKeys::LeftMouseButton); MapKeyTo(C, Flight.Fire, EKeys::SpaceBar);
	MapKeyTo(C, Flight.ToggleFA, EKeys::Z);
	MapKeyTo(C, Flight.Slipstream, EKeys::J);
	MapKeyTo(C, Flight.PowerBalanced, EKeys::One);
	MapKeyTo(C, Flight.PowerAttack, EKeys::Two);
	MapKeyTo(C, Flight.PowerTank, EKeys::Three);
	MapKeyTo(C, Flight.PowerRun, EKeys::Four);
	MapKeyTo(C, Flight.EqualizeShields, EKeys::X);
	MapKeyTo(C, Flight.RecenterVR, EKeys::Home);
	MapKeyTo(C, Flight.Menu, EKeys::Tab);

	// Xbox controller (XInput). Multiple keys on one Axis1D action resolve by highest
	// absolute value, so left stick Y and the triggers never fight: the larger input wins.
	const float DZ = FMath::Clamp(StickDeadZone, 0.f, 0.5f);
	const float Exp = FMath::Clamp(StickCurveExponent, 1.f, 4.f);
	MapAnalog(C, Flight.Thrust, EKeys::Gamepad_LeftY, DZ, Exp, EDeadZoneType::Axial);
	MapAnalog(C, Flight.Strafe, EKeys::Gamepad_LeftX, DZ, Exp, EDeadZoneType::Axial);
	MapAnalog(C, Flight.Thrust, EKeys::Gamepad_RightTriggerAxis, DZ * 0.5f, 1.f, EDeadZoneType::Axial);
	MapAnalog(C, Flight.Thrust, EKeys::Gamepad_LeftTriggerAxis, DZ * 0.5f, 1.f, EDeadZoneType::Axial, FVector(-1.f, -1.f, -1.f));
	// Right stick as one 2D input with a radial deadzone (no square-gate diagonals).
	MapAnalog(C, Flight.LookStick, EKeys::Gamepad_Right2D, DZ, Exp, EDeadZoneType::Radial);
	MapKeyTo(C, Flight.Roll, EKeys::Gamepad_RightShoulder); MapKeyTo(C, Flight.Roll, EKeys::Gamepad_LeftShoulder, -1.f);
	MapKeyTo(C, Flight.Vertical, EKeys::Gamepad_LeftThumbstick);           // L3 hold = up
	MapKeyTo(C, Flight.Vertical, EKeys::Gamepad_RightThumbstick, -1.f);    // R3 hold = down
	MapKeyTo(C, Flight.Fire, EKeys::Gamepad_FaceButton_Bottom);            // A
	MapKeyTo(C, Flight.ToggleFA, EKeys::Gamepad_FaceButton_Right);         // B
	MapKeyTo(C, Flight.EqualizeShields, EKeys::Gamepad_FaceButton_Left);   // X
	MapKeyTo(C, Flight.Slipstream, EKeys::Gamepad_FaceButton_Top);         // Y
	MapKeyTo(C, Flight.PowerBalanced, EKeys::Gamepad_DPad_Up);
	MapKeyTo(C, Flight.PowerAttack, EKeys::Gamepad_DPad_Right);
	MapKeyTo(C, Flight.PowerTank, EKeys::Gamepad_DPad_Down);
	MapKeyTo(C, Flight.PowerRun, EKeys::Gamepad_DPad_Left);
	MapKeyTo(C, Flight.RecenterVR, EKeys::Gamepad_Special_Left);           // View
	MapKeyTo(C, Flight.Menu, EKeys::Gamepad_Special_Right);                // Menu
}
