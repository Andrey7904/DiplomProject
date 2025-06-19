// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Parent_Plus.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Spawner_Soldiers_Blue_Plus.h" // Replace with the actual class header

// Sets default values
AAI_Parent_Plus::AAI_Parent_Plus()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAI_Parent_Plus::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAI_Parent_Plus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI_Parent_Plus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAI_Parent_Plus::ResetToRandomPoint()
{
    // Disable physics simulation on Mesh
    GetMesh()->SetSimulatePhysics(false);

    EnableInput(UGameplayStatics::GetPlayerController(this, 0));

    Reset_to_default();

    if (ActorHasTag(TEXT("Blue")))
    {
        ASpawner_Soldiers_Blue_Plus* Spawner = Cast<ASpawner_Soldiers_Blue_Plus>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawner_Soldiers_Blue_Plus::StaticClass()));
        if (!Spawner || !Spawner->Box) return;
        // Get random point in bounding box
        FVector Origin = Spawner->Box->GetComponentLocation();
        FVector Extent = Spawner->Box->GetScaledBoxExtent();
        FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

        float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        FVector NewLocation(RandomPoint.X, RandomPoint.Y, RandomPoint.Z + HalfHeight);

        FRotator NewRotation = FRotator(0.0f, UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f), 0.0f);

        // Teleport actor to new transform
        FTransform NewTransform(NewRotation, NewLocation, FVector(1.0f));
        SetActorTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);
    }
    else
    {
        ASpawner_Soldiers_Red_Plus* Spawner = Cast<ASpawner_Soldiers_Red_Plus>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawner_Soldiers_Red_Plus::StaticClass()));
        if (!Spawner || !Spawner->Box) return;
        // Get random point in bounding box
        FVector Origin = Spawner->Box->GetComponentLocation();
        FVector Extent = Spawner->Box->GetScaledBoxExtent();
        FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

        float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
        FVector NewLocation(RandomPoint.X, RandomPoint.Y, RandomPoint.Z + HalfHeight);

        FRotator NewRotation = FRotator(0.0f, UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f), 0.0f);

        // Teleport actor to new transform
        FTransform NewTransform(NewRotation, NewLocation, FVector(1.0f));
        SetActorTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);
    }
   
    GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);

}