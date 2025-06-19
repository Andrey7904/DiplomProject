
#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsTrainingEnvironment.h"
#include "LearningAgentsTrainerPlus.generated.h"

class AAI_Blue_Plus;
class ABP_AI_Red;

UCLASS()
class DIPLOMPROJECT_API ULearningAgentsTrainerPlus : public ULearningAgentsTrainingEnvironment
{
	GENERATED_BODY()
	
protected:
    virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;
    virtual void GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;
    virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;

private:
    void CalcAgentEfficiency();

    UPROPERTY()
    AAI_Blue_Plus* RewardActor;

    UPROPERTY()
    TArray<double> HitsAmountArray;

    UPROPERTY()
    double hitsSum = 0.0;

    UPROPERTY()
    double AvgHitsAmount = 0.0;
};
