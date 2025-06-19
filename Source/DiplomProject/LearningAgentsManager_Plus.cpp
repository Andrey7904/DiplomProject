
#include "LearningAgentsManager_Plus.h"
#include "LearningAgentsInteractorPlus.h"
#include "LearningAgentsTrainerPlus.h"
#include "AI_Blue_Plus.h"
#include "Kismet/GameplayStatics.h"
#include "LearningAgentsPolicy.h"
#include "LearningAgentsNeuralNetwork.h"

void ULearningAgentsManager_Plus::SetupAgents_Implementation()
{
    TArray<AActor*> Agents;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), AgentTag, Agents);

    for (AActor* Actor : Agents)
    {
        if (AAI_Blue_Plus* Agent = Cast<AAI_Blue_Plus>(Actor))
        {
            AddAgent(Agent);
        }
    }

    Interactor = MakeInteractor<ULearningAgentsInteractorPlus>(TEXT("AI_BlueInteractor"));
    Trainer = MakeTrainer<ULearningAgentsTrainerPlus>(TEXT("AI_BlueTrainer"));
    Policy = MakePolicy(Interactor, TEXT("AI_BluePolicy"));

    if (EncoderAsset.IsValid() && PolicyAsset.IsValid() && DecoderAsset.IsValid())
    {
        Policy->SetNeuralNetworkAssets(EncoderAsset.Get(), PolicyAsset.Get(), DecoderAsset.Get());
    }
}

void ULearningAgentsManager_Plus::TickAgents_Implementation(float DeltaTime)
{
    StepAgents(
        true,
        true,
        true,
        true,
        true);
}
