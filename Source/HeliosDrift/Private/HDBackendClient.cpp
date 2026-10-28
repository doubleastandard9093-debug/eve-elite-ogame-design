#include "HDBackendClient.h"
#include "HeliosDrift.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/ConfigCacheIni.h"

namespace
{
	FString ServerKey()
	{
		FString Key;
		if (GConfig) { GConfig->GetString(TEXT("HeliosDrift.Backend"), TEXT("ServerKey"), Key, GEngineIni); }
		return Key;
	}

	constexpr float RequestTimeoutSeconds = 10.f;

	void AddAuth(const TSharedRef<IHttpRequest, ESPMode::ThreadSafe>& Req)
	{
		const FString Key = ServerKey();
		if (!Key.IsEmpty()) { Req->SetHeader(TEXT("X-Helios-Server-Key"), Key); }
		Req->SetTimeout(RequestTimeoutSeconds);
	}

	FString ShipUrl(const FString& PilotId)
	{
		return FHDBackendClient::GetBaseUrl() + TEXT("/v1/pilots/") + FGenericPlatformHttp::UrlEncode(PilotId) + TEXT("/ship");
	}

	TArray<TSharedPtr<FJsonValue>> Vec3(double A, double B, double C)
	{
		TArray<TSharedPtr<FJsonValue>> Arr;
		Arr.Add(MakeShared<FJsonValueNumber>(A));
		Arr.Add(MakeShared<FJsonValueNumber>(B));
		Arr.Add(MakeShared<FJsonValueNumber>(C));
		return Arr;
	}

	bool ReadVec3(const TSharedPtr<FJsonObject>& Obj, const FString& Field, double Out[3])
	{
		const TArray<TSharedPtr<FJsonValue>>* Arr = nullptr;
		if (!Obj->TryGetArrayField(Field, Arr) || !Arr || Arr->Num() != 3) return false;
		for (int32 i = 0; i < 3; ++i)
		{
			double N = 0.0;
			if (!(*Arr)[i].IsValid() || !(*Arr)[i]->TryGetNumber(N) || !FMath::IsFinite(N)) return false;
			Out[i] = N;
		}
		return true;
	}
}

FString FHDBackendClient::GetBaseUrl()
{
	FString Url = TEXT("http://localhost:8080");
	if (GConfig) { GConfig->GetString(TEXT("HeliosDrift.Backend"), TEXT("BaseUrl"), Url, GEngineIni); }
	Url.RemoveFromEnd(TEXT("/"));
	return Url;
}

void FHDBackendClient::LoadShip(const FString& PilotId, FHDOnShipLoaded OnDone)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Req = FHttpModule::Get().CreateRequest();
	Req->SetURL(ShipUrl(PilotId));
	Req->SetVerb(TEXT("GET"));
	AddAuth(Req);
	Req->OnProcessRequestComplete().BindLambda([OnDone, PilotId](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnected)
	{
		FHDShipRecord Rec;
		Rec.PilotId = PilotId;
		if (!bConnected || !Response.IsValid())
		{
			UE_LOG(LogHelios, Warning, TEXT("Backend unreachable at %s (is docker compose up?)"), *FHDBackendClient::GetBaseUrl());
			OnDone.ExecuteIfBound(false, Rec);
			return;
		}
		const int32 Code = Response->GetResponseCode();
		if (Code == 404)
		{
			UE_LOG(LogHelios, Log, TEXT("No saved ship for %s (HTTP 404)"), *PilotId);
			OnDone.ExecuteIfBound(false, Rec);
			return;
		}
		if (Code < 200 || Code >= 300)
		{
			UE_LOG(LogHelios, Warning, TEXT("LoadShip %s failed: HTTP %d %s"), *PilotId, Code, *Response->GetContentAsString().Left(200));
			OnDone.ExecuteIfBound(false, Rec);
			return;
		}
		TSharedPtr<FJsonObject> Obj;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
		if (!FJsonSerializer::Deserialize(Reader, Obj) || !Obj.IsValid())
		{
			UE_LOG(LogHelios, Warning, TEXT("Bad ship JSON for %s"), *PilotId);
			OnDone.ExecuteIfBound(false, Rec);
			return;
		}
		Obj->TryGetStringField(TEXT("hull"), Rec.Hull);
		Obj->TryGetStringField(TEXT("system_id"), Rec.SystemId);
		double HP = Rec.HullHP;
		if (Obj->TryGetNumberField(TEXT("hull_hp"), HP) && FMath::IsFinite(HP)) { Rec.HullHP = (float)HP; }
		double V[3];
		if (ReadVec3(Obj, TEXT("location"), V)) { Rec.Location = FVector(V[0], V[1], V[2]); }
		if (ReadVec3(Obj, TEXT("rotation"), V)) { Rec.Rotation = FRotator(V[0], V[1], V[2]); }
		OnDone.ExecuteIfBound(true, Rec);
	});
	Req->ProcessRequest();
}

void FHDBackendClient::SaveShip(const FHDShipRecord& Record)
{
	TSharedPtr<FJsonObject> Obj = MakeShared<FJsonObject>();
	Obj->SetStringField(TEXT("pilot_id"), Record.PilotId);
	Obj->SetStringField(TEXT("hull"), Record.Hull);
	Obj->SetStringField(TEXT("system_id"), Record.SystemId);
	Obj->SetNumberField(TEXT("hull_hp"), Record.HullHP);
	Obj->SetArrayField(TEXT("location"), Vec3(Record.Location.X, Record.Location.Y, Record.Location.Z));
	Obj->SetArrayField(TEXT("rotation"), Vec3(Record.Rotation.Pitch, Record.Rotation.Yaw, Record.Rotation.Roll));

	FString Body;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Body);
	FJsonSerializer::Serialize(Obj.ToSharedRef(), Writer);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Req = FHttpModule::Get().CreateRequest();
	Req->SetURL(ShipUrl(Record.PilotId));
	Req->SetVerb(TEXT("PUT"));
	Req->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	AddAuth(Req);
	Req->SetContentAsString(Body);
	const FString Pilot = Record.PilotId;
	Req->OnProcessRequestComplete().BindLambda([Pilot](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnected)
	{
		const int32 Code = (bConnected && Response.IsValid()) ? Response->GetResponseCode() : 0;
		if (Code >= 200 && Code < 300)
		{
			UE_LOG(LogHelios, Log, TEXT("SaveShip %s -> HTTP %d"), *Pilot, Code);
		}
		else
		{
			UE_LOG(LogHelios, Warning, TEXT("SaveShip %s FAILED -> HTTP %d %s"), *Pilot, Code, Response.IsValid() ? *Response->GetContentAsString().Left(200) : TEXT("(no response)"));
		}
	});
	Req->ProcessRequest();
}
