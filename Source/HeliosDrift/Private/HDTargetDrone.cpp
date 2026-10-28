#include "HDTargetDrone.h"
#include "HDShipPowerComponent.h"
#include "HeliosDrift.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

AHDTargetDrone::AHDTargetDrone()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (Sphere.Succeeded()) { Mesh->SetStaticMesh(Sphere.Object); }
	Mesh->SetRelativeScale3D(FVector(4.f));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);

	Shields = CreateDefaultSubobject<UHDShieldComponent>(TEXT("Shields"));
	Shields->MaxPerFacing = 150.f;
	Shields->RegenPerSecond = 10.f;

	Tags.Add(TEXT("SlipTarget"));
}

void AHDTargetDrone::BeginPlay()
{
	Super::BeginPlay();
	Center = GetActorLocation();
	if (HasAuthority()) { Hull = MaxHull; }
}

void AHDTargetDrone::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!HasAuthority() || !bAlive || OrbitRadius <= 0.f) return;
	Angle = FMath::Fmod(Angle + OrbitDegreesPerSecond * DeltaSeconds, 360.f);
	const float Rad = FMath::DegreesToRadians(Angle);
	SetActorLocation(Center + FVector(FMath::Cos(Rad), FMath::Sin(Rad), 0.f) * OrbitRadius);
}

float AHDTargetDrone::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (!HasAuthority() || !bAlive || !(Damage > 0.f)) return 0.f;
	FVector Dir = DamageCauser ? (GetActorLocation() - DamageCauser->GetActorLocation()).GetSafeNormal() : FVector::ZeroVector;
	if (Dir.IsNearlyZero()) { Dir = -GetActorForwardVector(); }
	const float Overflow = Shields ? Shields->AbsorbDamage(Damage, Dir) : Damage;
	Hull = FMath::Max(0.f, Hull - Overflow);
	if (Hull <= 0.f)
	{
		UE_LOG(LogHelios, Log, TEXT("Drone %s destroyed"), *GetName());
		bAlive = false;
		ApplyAliveState();
		GetWorldTimerManager().SetTimer(RespawnTimer, this, &AHDTargetDrone::Respawn, RespawnSeconds, false);
	}
	return Damage;
}

void AHDTargetDrone::Respawn()
{
	Hull = MaxHull;
	if (Shields) { Shields->Refill(); }
	bAlive = true;
	ApplyAliveState();
}

void AHDTargetDrone::OnRep_Alive() { ApplyAliveState(); }

void AHDTargetDrone::ApplyAliveState()
{
	SetActorHiddenInGame(!bAlive);
	SetActorEnableCollision(bAlive);
}

void AHDTargetDrone::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHDTargetDrone, Hull);
	DOREPLIFETIME(AHDTargetDrone, bAlive);
}
