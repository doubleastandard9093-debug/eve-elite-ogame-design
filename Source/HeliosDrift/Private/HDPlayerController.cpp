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

	void MapKeyTo(UInputMappingContext* Ctx, UInputAction* Action, const FKey& Key, float Scale = 1.f, bool bDeadZone = false)
	{
		FEnhancedActionKeyMapping& M = Ctx->MapKey(Action, Key);
		if (bDeadZone)
		{
			M.Modifiers.Add(NewObject<UInputModifierDeadZone>(Ctx));
		}
		if (!FMath::IsNearlyEqual(Scale, 1.f))
		{
			UInputModifierScalar* S = NewObject<UInputModifierScalar>(Ctx);
			S->Scalar = FVector(Scale, Scale, Scale);
			M.Modifiers.Add(S);
		}
	}
}

void AHDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController()) { GetFlightInput(); }
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

	Flight.Thrust   = MakeAction(this, TEXT("IA_Thrust"), T::Axis1D);
	Flight.Strafe   = MakeAction(this, TEXT("IA_Strafe"), T::Axis1D);
	Flight.Vertical = MakeAction(this, TEXT("IA_Vertical"), T::Axis1D);
	Flight.Pitch    = MakeAction(this, TEXT("IA_Pitch"), T::Axis1D);
	Flight.Yaw      = MakeAction(this, TEXT("IA_Yaw"), T::Axis1D);
	Flight.Roll     = MakeAction(this, TEXT("IA_Roll"), T::Axis1D);
	Flight.Fire          = MakeAction(this, TEXT("IA_Fire"), T::Boolean);
	Flight.ToggleFA      = MakeAction(this, TEXT("IA_ToggleFA"), T::Boolean);
	Flight.Slipstream    = MakeAction(this, TEXT("IA_Slipstream"), T::Boolean);
	Flight.PowerBalanced = MakeAction(this, TEXT("IA_PowerBalanced"), T::Boolean);
	Flight.PowerAttack   = MakeAction(this, TEXT("IA_PowerAttack"), T::Boolean);
	Flight.PowerTank     = MakeAction(this, TEXT("IA_PowerTank"), T::Boolean);
	Flight.PowerRun      = MakeAction(this, TEXT("IA_PowerRun"), T::Boolean);
	Flight.EqualizeShields = MakeAction(this, TEXT("IA_EqualizeShields"), T::Boolean);
	Flight.RecenterVR    = MakeAction(this, TEXT("IA_RecenterVR"), T::Boolean);

	// Keyboard + mouse
	MapKeyTo(C, Flight.Thrust, EKeys::W);            MapKeyTo(C, Flight.Thrust, EKeys::S, -1.f);
	MapKeyTo(C, Flight.Strafe, EKeys::D);            MapKeyTo(C, Flight.Strafe, EKeys::A, -1.f);
	MapKeyTo(C, Flight.Vertical, EKeys::R);          MapKeyTo(C, Flight.Vertical, EKeys::F, -1.f);
	MapKeyTo(C, Flight.Roll, EKeys::E);              MapKeyTo(C, Flight.Roll, EKeys::Q, -1.f);
	MapKeyTo(C, Flight.Pitch, EKeys::MouseY, 0.1f);
	MapKeyTo(C, Flight.Yaw, EKeys::MouseX, 0.1f);
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

	// Gamepad (XInput; also most HOTAS via Steam Input / XInput wrappers)
	MapKeyTo(C, Flight.Thrust, EKeys::Gamepad_LeftY, 1.f, true);
	MapKeyTo(C, Flight.Strafe, EKeys::Gamepad_LeftX, 1.f, true);
	MapKeyTo(C, Flight.Pitch, EKeys::Gamepad_RightY, 1.f, true);
	MapKeyTo(C, Flight.Yaw, EKeys::Gamepad_RightX, 1.f, true);
	MapKeyTo(C, Flight.Roll, EKeys::Gamepad_RightShoulder); MapKeyTo(C, Flight.Roll, EKeys::Gamepad_LeftShoulder, -1.f);
	MapKeyTo(C, Flight.Vertical, EKeys::Gamepad_DPad_Up);   MapKeyTo(C, Flight.Vertical, EKeys::Gamepad_DPad_Down, -1.f);
	MapKeyTo(C, Flight.Fire, EKeys::Gamepad_RightTrigger);
	MapKeyTo(C, Flight.ToggleFA, EKeys::Gamepad_LeftThumbstick);
	MapKeyTo(C, Flight.Slipstream, EKeys::Gamepad_FaceButton_Top);
	MapKeyTo(C, Flight.EqualizeShields, EKeys::Gamepad_FaceButton_Right);
	MapKeyTo(C, Flight.PowerBalanced, EKeys::Gamepad_DPad_Left);
	MapKeyTo(C, Flight.PowerAttack, EKeys::Gamepad_DPad_Right);
	MapKeyTo(C, Flight.RecenterVR, EKeys::Gamepad_Special_Left);
}
