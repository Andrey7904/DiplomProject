
#include "Drone_Plus.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


ADrone_Plus::ADrone_Plus()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADrone_Plus::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &ADrone_Plus::OnDroneTakeAnyDamage);
	
}

void ADrone_Plus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADrone_Plus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADrone_Plus::OnDroneTakeAnyDamage(
    AActor* DamagedActor,
    float Damage,
    const UDamageType* DamageType,
    AController* InstigatedBy,
    AActor* DamageCauser
)
{
    FVector ExplosionLocation = GetActorLocation();

    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionEffect,
            ExplosionLocation,
            FRotator::ZeroRotator,
            FVector(1.0f),
            true
        );
    }

    SetDead();

    // Обчислюємо кількість пошкоджених ворогів
    CalcEnemyHits(ExplosionLocation);

    UGameplayStatics::ApplyRadialDamage(
        GetWorld(),
        RadialDamage,
        ExplosionLocation,
        DamageRadius,
        nullptr,
        TArray<AActor*>(),
        this,
        InstigatedBy,
        true // DoFullDamage
    );

    GetWorldTimerManager().SetTimer(
        ResetTimerHandle,
        this,
        &ADrone_Plus::ResetToRandomPoint,
        3.0f,
        false
    );
}

void ADrone_Plus::SetDead()
{
    bIsDead = true;

    if (Drone)
    {
        Drone->SetVisibility(false, true); // Пропагує до дітей
        Drone->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (Mesh)
    {
        Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    if (UCharacterMovementComponent* Movement = GetCharacterMovement())
    {
        Movement->DisableMovement();
    }

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        DisableInput(PlayerController);
    }
}


// BP Ref
void ADrone_Plus::CalcEnemyHits(FVector ExplosionLocation)
{}

// BP Ref
void ADrone_Plus::ResetToRandomPoint()
{}