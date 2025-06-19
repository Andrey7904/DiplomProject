// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Drone_Red_Plus.generated.h"

UCLASS()
class DIPLOMPROJECT_API ADrone_Red_Plus : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADrone_Red_Plus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void MinDistanceSimple(float& OutDistance, TArray<AActor*>& OutSeenEnemies);

	UFUNCTION(BlueprintCallable)
	AActor* FindNearestEnemyFromArray(const TArray<AActor*>& SeenEnemies);

	UFUNCTION(BlueprintCallable)
	void TraceUnder();

	UFUNCTION(BlueprintCallable)
	void MoveToEnemy();

	UFUNCTION(BlueprintCallable)
	void AvoidObstacles();

	UPROPERTY(BlueprintReadWrite)
	AActor* NearestEnemy;

	UPROPERTY(BlueprintReadWrite)
	bool bValidNearestEnemyReceived;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    AActor* NearestEnemy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoveSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool MovingAroundObstacleUp = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool MovingAroundObstacleForward = false;

    UPROPERTY(BlueprintReadWrite)
    bool DroppedVelocityOverEnemy = false;

    UPROPERTY(BlueprintReadWrite)
    FVector TempLocation;

    UPROPERTY(BlueprintReadWrite)
    float TempMoveTargetHeight = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float TempRandomFloat = 0.f;

    UPROPERTY(BlueprintReadWrite)
    float TempScaleValue = 1.f;

    UPROPERTY(BlueprintReadWrite)
    float TempScaleValueXY = 1.f;

    UPROPERTY(BlueprintReadWrite)
    float TempScaleValueXX = 1.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseRandomHeight = false;

    // Map limits (used for clamping)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MapMinX = -4650.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MapMaxX = 4940.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MapMinY = -7250.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MapMaxY = 4770.f;

private:
    FVector MapLimitClamp(const FVector& InLocation);
};