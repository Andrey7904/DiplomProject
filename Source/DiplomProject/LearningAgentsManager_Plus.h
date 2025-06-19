// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsManager.h"
#include "LearningAgentsManager_Plus.generated.h"

class AAI_Blue_Plus;
class ULearningAgentsInteractorPlus;
class ULearningAgentsTrainerPlus;
class ULearningAgentsPolicy;
class ULearningAgentsNeuralNetwork;

UCLASS()
class DIPLOMPROJECT_API ULearningAgentsManager_Plus : public ULearningAgentsManager
{
	GENERATED_BODY()
	
protected:
    virtual void SetupAgents_Implementation();
    virtual void TickAgents_Implementation(float DeltaTime);

private:
    UPROPERTY()
    ULearningAgentsInteractorPlus* Interactor;

    UPROPERTY()
    ULearningAgentsTrainerPlus* Trainer;

    UPROPERTY()
    ULearningAgentsPolicy* Policy;

    UPROPERTY(EditAnywhere, Category = "Learning")
    TSoftObjectPtr<ULearningAgentsNeuralNetwork> EncoderAsset;

    UPROPERTY(EditAnywhere, Category = "Learning")
    TSoftObjectPtr<ULearningAgentsNeuralNetwork> PolicyAsset;

    UPROPERTY(EditAnywhere, Category = "Learning")
    TSoftObjectPtr<ULearningAgentsNeuralNetwork> DecoderAsset;

    UPROPERTY(EditAnywhere)
    FName AgentTag = FName("Blue");
};
