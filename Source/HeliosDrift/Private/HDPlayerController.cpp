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

	void Map(UInputMappingContext* Ctx, UInputAction* Action, const FKey& Key, float Scale = 1.f, bool bDeadZone = false)
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
	Map(C, Flight.Thrust, EKeys::W);            Map(C, Flight.Thrust, EKeys::S, -1.f);
	Map(C, Flight.Strafe, EKeys::D);            Map(C, Flight.Strafe, EKeys::A, -1.f);
	Map(C, Flight.Vertical, EKeys::R);          Map(C, Flight.Vertical, EKeys::F, -1.f);
	Map(C, Flight.Roll, EKeys::E);              Map(C, Flight.Roll, EKeys::Q, -1.f);
	Map(C, Flight.Pitch, EKeys::MouseY, 0.1f);
	Map(C, Flight.Yaw, EKeys::MouseX, 0.1f);
	Map(C, Flight.Pitch, EKeys::Up);            Map(C, Flight.Pitch, EKeys::Down, -1.f);
	Map(C, Flight.Yaw, EKeys::Right);           Map(C, Flight.Yaw, EKeys::Left, -1.f);
	Map(C, Flight.Fire, EKeys::LeftMouseButton); Map(C, Flight.Fire, EKeys::SpaceBar);
	Map(C, Flight.ToggleFA, EKeys::Z);
	Map(C, Flight.Slipstream, EKeys::J);
	Map(C, Flight.PowerBalanced, EKeys::One);
	Map(C, Flight.PowerAttack, EKeys::Two);
	Map(C, Flight.PowerTank, EKeys::Three);
	Map(C, Flight.PowerRun, EKeys::Four);
	Map(C, Flight.EqualizeShields, EKeys::X);
	Map(C, Flight.RecenterVR, EKeys::Home);

	// Gamepad (XInput; also most HOTAS via Steam Input / XInput wrappers)
	Map(C, Flight.Thrust, EKeys::Gamepad_LeftY, 1.f, true);
	Map(C, Flight.Strafe, EKeys::Gamepad_LeftX, 1.f, true);
	Map(C, Flight.Pitch, EKeys::Gamepad_RightY, 1.f, true);
	Map(C, Flight.Yaw, EKeys::Gamepad_RightX, 1.f, true);
	Map(C, Flight.Roll, EKeys::Gamepad_RightShoulder); Map(C, Flight.Roll, EKeys::Gamepad_LeftShoulder, -1.f);
	Map(C, Flight.Vertical, EKeys::Gamepad_DPad_Up);   Map(C, Flight.Vertical, EKeys::Gamepad_DPad_Down, -1.f);
	Map(C, Flight.Fire, EKeys::Gamepad_RightTrigger);
	Map(C, Flight.ToggleFA, EKeys::Gamepad_LeftThumbstick);
	Map(C, Flight.Slipstream, EKeys::Gamepad_FaceButton_Top);
	Map(C, Flight.EqualizeShields, EKeys::Gamepad_FaceButton_Right);
	Map(C, Flight.PowerBalanced, EKeys::Gamepad_DPad_Left);
	Map(C, Flight.PowerAttack, EKeys::Gamepad_DPad_Right);
	Map(C, Flight.RecenterVR, EKeys::Gamepad_Special_Left);
}
