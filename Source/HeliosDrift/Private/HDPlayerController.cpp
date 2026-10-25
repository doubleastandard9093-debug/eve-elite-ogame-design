#include "HDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void AHDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController() && FlightMapping)
	{
		if (auto* Sub = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Sub->AddMappingContext(FlightMapping, 0);
		}
	}
}
