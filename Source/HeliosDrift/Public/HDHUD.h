#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HDHUD.generated.h"

/** Debug flight HUD for the M1 slice (canvas text). Replaced by diegetic UMG panels later. */
UCLASS()
class HELIOSDRIFT_API AHDHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void DrawHUD() override;
};
