// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner_Soldiers_Blue_Plus.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "AI_Parent_Plus.h"

// Sets default values
ASpawner_Soldiers_Blue_Plus::ASpawner_Soldiers_Blue_Plus()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawner_Soldiers_Blue_Plus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawner_Soldiers_Blue_Plus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// BP_Spawner_Soldiers_Blue.cpp



ASpawner_Soldiers_Blue_Plus::ASpawner_Soldiers_Blue_Plus()
{
    PrimaryActorTick.bCanEverTick = false;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    RootComponent = Box;
}

void ASpawner_Soldiers_Blue_Plus::BeginPlay()
{
    Super::BeginPlay();
}

void ASpawner_Soldiers_Blue_Plus::SpawnBlueSoldiers()
{
    int32 Count = 5;
    for (int32 i = 0; i < Count; ++i)
    {
        FVector Origin = Box->GetComponentLocation();
        FVector Extent = Box->GetScaledBoxExtent();
        FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

        UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
        FVector NavLocation;
        if (NavSys && NavSys->K2_GetRandomReachablePointInRadius(this, RandomPoint, NavLocation, 1000.f))
        {
            FTransform SpawnTransform;
            SpawnTransform.SetLocation(NavLocation);
            SpawnTransform.SetRotation(FRotator(0.f, 90.f, 0.f).Quaternion());
            SpawnTransform.SetScale3D(FVector(1.f));

            GetWorld()->SpawnActor<AAI_Parent_Plus>(AAI_Parent_Plus::StaticClass(), SpawnTransform);
        }
    }
}
