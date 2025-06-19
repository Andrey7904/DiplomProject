// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone_Red_Plus.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ADrone_Red_Plus::ADrone_Red_Plus()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADrone_Red_Plus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrone_Red_Plus::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 1. Set Valid Nearest Enemy Received = false
    bValidNearestEnemyReceived = false;


    TArray<AActor*> SeenEnemies;
    float Distance = 0.0f;
    MinDistanceSimple(Distance, SeenEnemies);

    AActor* NearestEnemy = FindNearestEnemyFromArray(SeenEnemies);
    if (IsValid(NearestEnemy))
    {
        this->NearestEnemy = NearestEnemy;
        bValidNearestEnemyReceived = true;
    }
    else
    {
        bValidNearestEnemyReceived = false;
    }

    if (bValidNearestEnemyReceived)
    {
        TraceUnder();
    }

    if (bValidNearestEnemyReceived)
    {
        MoveToEnemy();
    }

    AvoidObstacles();
}


// Called to bind functionality to input
void ADrone_Red_Plus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADrone_Red_Plus::MoveToEnemy()
{
    if (!NearestEnemy || MovingAroundObstacleUp || MovingAroundObstacleForward)
        return;

    FVector SelfLoc = GetActorLocation();
    FVector EnemyLoc = NearestEnemy->GetActorLocation();

    float DistX = FMath::Abs(SelfLoc.X - EnemyLoc.X);
    float DistY = FMath::Abs(SelfLoc.Y - EnemyLoc.Y);
    float DistZ = FMath::Abs(SelfLoc.Z - EnemyLoc.Z);
    float Dist3D = FVector::Dist(SelfLoc, EnemyLoc);

    bool bCloseXYZ = (DistX <= 1000.f && DistY <= 1000.f && DistZ <= 1300.f);
    bool bCloseDist = Dist3D <= 3500.f;

    if (!(bCloseXYZ || bCloseDist))
        return;
    TempMoveTargetHeight = bUseRandomHeight ? 3000.f : 0.f;
    TempRandomFloat = FMath::FRandRange(-300.f, 300.f);

    TempScaleValue = 1.f;
    TempScaleValueXY = 1.f;
    TempScaleValueXX = 1.f;

    DroppedVelocityOverEnemy = GetVelocity().IsNearlyZero();

    FVector AdjustedTarget = EnemyLoc + FVector(0.f, 0.f, TempMoveTargetHeight + TempRandomFloat);
    FVector Direction = (AdjustedTarget - SelfLoc).GetSafeNormal();
    TempLocation = Direction;

    FVector ClampedTarget = MapLimitClamp(AdjustedTarget);

    AddMovementInput(TempLocation * TempScaleValue, 1.0f, true);

    FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(SelfLoc, EnemyLoc);
    FRotator SmoothRot = UKismetMathLibrary::RLerp(GetActorRotation(), LookAtRot, 0.1f, true);
    SetActorRotation(FRotator(0.f, SmoothRot.Yaw, 0.f));
}

FVector ADrone_Red_Plus::MapLimitClamp(const FVector& InLocation)
{
    float X = FMath::Clamp(InLocation.X, MapMinX, MapMaxX);
    float Y = FMath::Clamp(InLocation.Y, MapMinY, MapMaxY);
    return FVector(X, Y, InLocation.Z);
}