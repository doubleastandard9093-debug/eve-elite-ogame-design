#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HDPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class HELIOSDRIFT_API AHDPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	/** Assign in a Blueprint subclass (BP_HDPlayerController). */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> FlightMapping;
};
