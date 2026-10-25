#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HDShipMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EHDTravelMode : uint8 { Normal, Spooling, Slipstream };

/** One client input frame, sent to the server. */
USTRUCT()
struct FHDShipMove
{
	GENERATED_BODY()
	UPROPERTY() float Timestamp = 0.f;
	UPROPERTY() float DeltaTime = 0.f;
	/** X forward, Y right, Z up, each -1..1 */
	UPROPERTY() FVector Linear = FVector::ZeroVector;
	/** X pitch, Y yaw, Z roll, each -1..1 */
	UPROPERTY() FVector Rot = FVector::ZeroVector;
	UPROPERTY() bool bFlightAssist = true;
};

/** Authoritative kinematic state. */
USTRUCT()
struct FHDShipState
{
	GENERATED_BODY()
	UPROPERTY() float Timestamp = 0.f;
	UPROPERTY() FVector Location = FVector::ZeroVector;
	UPROPERTY() FQuat Rotation = FQuat::Identity;
	UPROPERTY() FVector Velocity = FVector::ZeroVector;
	UPROPERTY() FVector AngularVelocity = FVector::ZeroVector;
};

/**
 * 6DoF ship movement (doc 02). Owning client predicts and sends moves; server simulates
 * authoritatively and acks; client rewinds to the ack and replays unacked moves.
 * Simulated proxies interpolate toward replicated ServerState.
 */
UCLASS(ClassGroup = (Ship), meta = (BlueprintSpawnableComponent))
class HELIOSDRIFT_API UHDShipMovementComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UHDShipMovementComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Local input, written by the pawn's input handlers. */
	FVector InputLinear = FVector::ZeroVector;
	FVector InputRot = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly, Category = "Flight") bool bFlightAssist = true;
	UFUNCTION(BlueprintCallable, Category = "Flight") void ToggleFlightAssist() { bFlightAssist = !bFlightAssist; }

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Flight") void ServerToggleSlipstream();
	/** Server: leave slipstream/spool. Interdicted adds a cooldown. */
	void DropOut(bool bInterdicted);

	UFUNCTION(BlueprintPure, Category = "Flight") EHDTravelMode GetTravelMode() const { return TravelMode; }
	UFUNCTION(BlueprintPure, Category = "Flight") FVector GetShipVelocity() const { return Velocity; }
	UFUNCTION(BlueprintPure, Category = "Flight") float GetSpeedCap() const;
	UFUNCTION(BlueprintPure, Category = "Flight") float GetSpoolRemaining() const { return SpoolRemaining; }

	// Tuning (cm, cm/s, deg/s)
	UPROPERTY(EditAnywhere, Category = "Flight") float BaseMaxSpeed = 30000.f;
	UPROPERTY(EditAnywhere, Category = "Flight") float LinearAccel = 15000.f;
	UPROPERTY(EditAnywhere, Category = "Flight") FVector MaxAngularRate = FVector(60.f, 45.f, 90.f);
	UPROPERTY(EditAnywhere, Category = "Flight") float AngularAccel = 180.f;
	UPROPERTY(EditAnywhere, Category = "Travel") float SlipSpeed = 2000000.f;
	UPROPERTY(EditAnywhere, Category = "Travel") float SpoolTime = 4.f;
	UPROPERTY(EditAnywhere, Category = "Travel") float DropDistance = 50000.f;
	UPROPERTY(EditAnywhere, Category = "Travel") float InterdictCooldown = 10.f;

protected:
	UFUNCTION(Server, Unreliable) void ServerMove(const FHDShipMove& Move);
	UFUNCTION(Client, Unreliable) void ClientAck(const FHDShipState& State);

	UPROPERTY(Replicated) EHDTravelMode TravelMode = EHDTravelMode::Normal;
	UPROPERTY(Replicated) FHDShipState ServerState;

private:
	void SimulateMove(const FHDShipMove& Move);
	void UpdateTravelAuthority(float DeltaTime);
	AActor* PickSlipTarget() const;
	FHDShipState CaptureState(float Timestamp) const;

	FVector Velocity = FVector::ZeroVector;
	FVector AngularVelocity = FVector::ZeroVector;
	TArray<FHDShipMove> Unacked;
	float ClientTime = 0.f;
	float LastServerTimestamp = -1.f;
	float LastAckTimestamp = -1.f;
	float SpoolRemaining = 0.f;
	float SlipCooldown = 0.f;
	TWeakObjectPtr<AActor> TravelTarget;
};
