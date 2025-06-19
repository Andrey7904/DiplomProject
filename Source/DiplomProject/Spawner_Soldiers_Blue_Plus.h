// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "Spawner_Soldiers_Blue_Plus.generated.h"



UCLASS()
class DIPLOMPROJECT_API ASpawner_Soldiers_Blue_Plus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner_Soldiers_Blue_Plus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Box;

	// Спавн солдатів
	UFUNCTION(BlueprintCallable)
	void SpawnBlueSoldiers();
};
