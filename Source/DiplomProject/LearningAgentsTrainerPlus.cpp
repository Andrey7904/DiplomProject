// Fill out your copyright notice in the Description page of Project Settings.
#include "AI_Blue_Plus.h"
#include "Kismet/GameplayStatics.h"

#include "LearningAgentsTrainerPlus.h"

void ULearningAgentsTrainerPlus::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
    AAI_Blue_Plus* Agent = Cast<AAI_Blue_Plus>(GetAgent(AgentId));
    if (!IsValid(Agent))
    {
        OutReward = 0.f;
        return;
    }

    RewardActor = Agent;
    CalcAgentEfficiency();

    OutReward = (RewardActor && RewardActor->AmountOfHits > 0) ? 1.0f : 0.0f;
}

void ULearningAgentsTrainerPlus::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId)
{
    const AAI_Blue_Plus* Agent = Cast<AAI_Blue_Plus>(GetAgent(AgentId));
    if (!Agent)
    {
        OutCompletion = ELearningAgentsCompletion::Running;
        return;
    }

    OutCompletion = Agent->IsDead() ? ELearningAgentsCompletion::Termination : ELearningAgentsCompletion::Running;
}

void ULearningAgentsTrainerPlus::ResetAgentEpisode_Implementation(const int32 AgentId)
{
    AAI_Blue_Plus* Agent = Cast<AAI_Blue_Plus>(GetAgent(AgentId));
    if (Agent)
    {
        Agent->ResetToRandomPoint();
    }
}

void ULearningAgentsTrainerPlus::CalcAgentEfficiency()
{
    HitsAmountArray.Empty();
    hitsSum = 0.0;

    TArray<AActor*> FoundRedAgents;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABP_AI_Red::StaticClass(), FoundRedAgents);

    for (AActor* Actor : FoundRedAgents)
    {
        ABP_AI_Red* Red = Cast<ABP_AI_Red>(Actor);
        if (!Red) continue;

        double Hits = Red->AmountOfHits;
        HitsAmountArray.Add(Hits);
        hitsSum += Hits;
    }

    AvgHitsAmount = HitsAmountArray.Num() > 0 ? hitsSum / HitsAmountArray.Num() : 0.0;
}