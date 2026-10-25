#include "HDGameMode.h"
#include "HDShipPawn.h"
#include "HDPlayerController.h"
#include "HDHUD.h"
#include "HDBackendClient.h"
#include "HeliosDrift.h"
#include "GameFramework/PlayerState.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

AHDGameMode::AHDGameMode()
{
	DefaultPawnClass = AHDShipPawn::StaticClass();
	PlayerControllerClass = AHDPlayerController::StaticClass();
	HUDClass = AHDHUD::StaticClass();
	FString Sys;
	if (FParse::Value(FCommandLine::Get(), TEXT("SystemId="), Sys)) { SystemId = FName(*Sys); }
}

FString AHDGameMode::PilotIdFor(const AController* C)
{
	const APlayerState* PS = C ? C->PlayerState : nullptr;
	if (!PS) return TEXT("pilot-unknown");
	if (PS->GetUniqueId().IsValid()) { return PS->GetUniqueId().ToString(); }
	return PS->GetPlayerName();
}

void AHDGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	const FString Pilot = PilotIdFor(NewPlayer);
	UE_LOG(LogHelios, Log, TEXT("Pilot %s joined system %s"), *Pilot, *SystemId.ToString());

	TWeakObjectPtr<APlayerController> WeakPC(NewPlayer);
	const FString Sys = SystemId.ToString();
	FHDBackendClient::LoadShip(Pilot, FHDOnShipLoaded::CreateLambda([WeakPC, Sys](bool bFound, const FHDShipRecord& Rec)
	{
		if (!bFound || !WeakPC.IsValid()) return;
		AHDShipPawn* Ship = Cast<AHDShipPawn>(WeakPC->GetPawn());
		if (!Ship) return;
		if (Rec.HullHP > 0.f) { Ship->HullHP = Rec.HullHP; }
		if (Rec.SystemId == Sys && !Rec.Location.IsNearlyZero())
		{
			Ship->SetActorLocationAndRotation(Rec.Location, Rec.Rotation);
		}
		UE_LOG(LogHelios, Log, TEXT("Loaded ship for %s: hull=%s hp=%.0f"), *Rec.PilotId, *Rec.Hull, Rec.HullHP);
	}));
}

void AHDGameMode::Logout(AController* Exiting)
{
	if (const AHDShipPawn* Ship = Exiting ? Cast<AHDShipPawn>(Exiting->GetPawn()) : nullptr)
	{
		FHDShipRecord Rec;
		Rec.PilotId = PilotIdFor(Exiting);
		Rec.SystemId = SystemId.ToString();
		Rec.Location = Ship->GetActorLocation();
		Rec.Rotation = Ship->GetActorRotation();
		Rec.HullHP = Ship->HullHP;
		FHDBackendClient::SaveShip(Rec);
	}
	Super::Logout(Exiting);
}
