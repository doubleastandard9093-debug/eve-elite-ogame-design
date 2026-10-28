#include "HDHUD.h"
#include "HDShipPawn.h"
#include "HDShipMovementComponent.h"
#include "HDShipPowerComponent.h"
#include "HDPlayerController.h"
#include "Engine/Canvas.h"

void AHDHUD::DrawHUD()
{
	Super::DrawHUD();
	const AHDShipPawn* Ship = Cast<AHDShipPawn>(GetOwningPawn());
	if (!Ship || !Canvas || !Ship->Movement || !Ship->Power || !Ship->Shields) return;

	const UHDShipMovementComponent* M = Ship->Movement;
	const UHDShipPowerComponent* P = Ship->Power;
	const UHDShieldComponent* S = Ship->Shields;

	const TCHAR* Mode = TEXT("NORMAL");
	if (M->GetTravelMode() == EHDTravelMode::Spooling) Mode = TEXT("SPOOLING");
	else if (M->GetTravelMode() == EHDTravelMode::Slipstream) Mode = TEXT("SLIPSTREAM");

	TArray<FString> Lines;
	Lines.Add(FString::Printf(TEXT("SPEED %.0f m/s  (cap %.0f)"), M->GetShipVelocity().Size() / 100.f, M->GetSpeedCap() / 100.f));
	Lines.Add(FString::Printf(TEXT("FLIGHT ASSIST %s   TRAVEL %s"), M->bFlightAssist ? TEXT("ON") : TEXT("OFF"), Mode));
	Lines.Add(FString::Printf(TEXT("POWER  W %.0f  S %.0f  E %.0f  A %.0f"),
		P->GetPower(EHDSubsystem::Weapons), P->GetPower(EHDSubsystem::Shields), P->GetPower(EHDSubsystem::Engines), P->GetPower(EHDSubsystem::Auxiliary)));
	Lines.Add(FString::Printf(TEXT("SHIELDS  F %.0f  A %.0f  P %.0f  S %.0f"),
		S->GetFacing(EHDShieldFacing::Fore), S->GetFacing(EHDShieldFacing::Aft), S->GetFacing(EHDShieldFacing::Port), S->GetFacing(EHDShieldFacing::Starboard)));
	Lines.Add(FString::Printf(TEXT("HULL %.0f"), Ship->HullHP));

	float Y = 40.f;
	for (const FString& L : Lines)
	{
		DrawText(L, FLinearColor(0.4f, 0.9f, 1.f), 40.f, Y);
		Y += 20.f;
	}
	const AHDPlayerController* PC = Cast<AHDPlayerController>(GetOwningPlayerController());
	if (PC && PC->IsMenuOpen())
	{
		static const TCHAR* Help[] = {
			TEXT("CONTROLS  (Tab / Menu to close)"),
			TEXT("Thrust/strafe: W S A D  | left stick, RT fwd, LT reverse"),
			TEXT("Up/down: R F  | hold L3 up, hold R3 down"),
			TEXT("Pitch/yaw: mouse, arrows  | right stick    Roll: Q E  | LB RB"),
			TEXT("Fire: LMB, Space  | A      Flight assist: Z  | B"),
			TEXT("Shields equalize: X  | X     Slipstream: J  | Y"),
			TEXT("Power Bal/Atk/Tank/Run: 1 2 3 4  | D-pad up/right/down/left"),
			TEXT("Recenter VR: Home  | View"),
		};
		Y += 20.f;
		for (const TCHAR* L : Help)
		{
			DrawText(FString(L), FLinearColor(1.f, 0.85f, 0.4f), 40.f, Y);
			Y += 20.f;
		}
	}

	// crosshair
	const float CX = Canvas->ClipX * 0.5f, CY = Canvas->ClipY * 0.5f;
	DrawLine(CX - 10.f, CY, CX + 10.f, CY, FLinearColor::White);
	DrawLine(CX, CY - 10.f, CX, CY + 10.f, FLinearColor::White);
}
