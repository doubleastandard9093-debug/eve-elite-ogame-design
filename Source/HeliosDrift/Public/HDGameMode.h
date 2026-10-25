#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HDGameMode.generated.h"

/** Server-authoritative rules for one star-system zone. */
UCLASS()
class HELIOSDRIFT_API AHDGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AHDGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	/** Zone/system id this server instance simulates (from -SystemId= cmdline). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zone")
	FName SystemId = TEXT("sys-0001");

	static FString PilotIdFor(const AController* C);
};
