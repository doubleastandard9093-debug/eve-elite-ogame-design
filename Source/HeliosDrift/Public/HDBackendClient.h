#pragma once
#include "CoreMinimal.h"

/** Ship record as stored by the backend (/v1/pilots/{id}/ship). */
struct FHDShipRecord
{
	FString PilotId;
	FString Hull = TEXT("skiff");
	FString SystemId;
	FVector Location = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	float HullHP = 1000.f;
};

DECLARE_DELEGATE_TwoParams(FHDOnShipLoaded, bool /*bFound*/, const FHDShipRecord& /*Record*/);

/** Minimal HTTP client for the persistent backend. Server-side only. Config: [HeliosDrift.Backend] BaseUrl / ServerKey in DefaultEngine.ini */
class HELIOSDRIFT_API FHDBackendClient
{
public:
	static FString GetBaseUrl();
	static void LoadShip(const FString& PilotId, FHDOnShipLoaded OnDone);
	static void SaveShip(const FHDShipRecord& Record);
};
