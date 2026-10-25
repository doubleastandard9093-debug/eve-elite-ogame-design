#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HDTargetDrone.generated.h"

class UStaticMeshComponent;
class UHDShieldComponent;

/** Simple target: orbits its spawn point, has 4-facing shields + hull, respawns after destruction. Tagged "SlipTarget". */
UCLASS()
class HELIOSDRIFT_API AHDTargetDrone : public AActor
{
	GENERATED_BODY()
public:
	AHDTargetDrone();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone") TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drone") TObjectPtr<UHDShieldComponent> Shields;

	UPROPERTY(EditAnywhere, Category = "Drone") float MaxHull = 300.f;
	UPROPERTY(EditAnywhere, Category = "Drone") float OrbitRadius = 3000.f;
	UPROPERTY(EditAnywhere, Category = "Drone") float OrbitDegreesPerSecond = 10.f;
	UPROPERTY(EditAnywhere, Category = "Drone") float RespawnSeconds = 5.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Drone") float Hull = 300.f;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Alive) bool bAlive = true;
	UFUNCTION() void OnRep_Alive();

private:
	void ApplyAliveState();
	void Respawn();
	FVector Center = FVector::ZeroVector;
	float Angle = 0.f;
	FTimerHandle RespawnTimer;
};
