#include "HDGameMode.h"
#include "HDShipPawn.h"
#include "HDPlayerController.h"
#include "HeliosDrift.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

AHDGameMode::AHDGameMode()
{
	DefaultPawnClass = AHDShipPawn::StaticClass();
	PlayerControllerClass = AHDPlayerController::StaticClass();
	FString Sys;
	if (FParse::Value(FCommandLine::Get(), TEXT("SystemId="), Sys)) { SystemId = FName(*Sys); }
}

void AHDGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogHelios, Log, TEXT("Pilot joined system %s"), *SystemId.ToString());
	// TODO(M0/M2): exchange EOS token for backend session, load ship from backend.
}
