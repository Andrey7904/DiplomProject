// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Drone_Plus.generated.h"

UCLASS()
class DIPLOMPROJECT_API ADrone_Plus : public ACharacter
{
    GENERATED_BODY()

public:
    ADrone_Plus();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnDroneTakeAnyDamage(
        AActor* DamagedActor,
        float Damage,
        const UDamageType* DamageType,
        AController* InstigatedBy,
        AActor* DamageCauser
    );

    UFUNCTION()
    void SetDead();

    UFUNCTION()
    void CalcEnemyHits(FVector ExplosionLocation);

    UFUNCTION()
    void ResetToRandomPoint();

    UPROPERTY(EditAnywhere)
    class UParticleSystem* ExplosionEffect;

    UPROPERTY(EditAnywhere)
    float RadialDamage = 2.0f;

    UPROPERTY(EditAnywhere)
    float DamageRadius = 250.0f;

    FTimerHandle ResetTimerHandle;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bIsDead = false;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Drone;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* Mesh;


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
