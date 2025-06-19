
#include "LearningAgentsInteractorPlus.h"

#include "AI_Blue_Plus.h"
#include "LearningAgents.h"
#include "LearningAgentsObservations.h"
#include "LearningAgentsActions.h"

void ULearningAgentsInteractorPlus::SpecifyAgentObservation_Implementation(FLearningAgentsObservationSchemaElement& OutObservationSchemaElement, ULearningAgentsObservationSchema* InObservationSchema)
{
    // Character observation
    FLearningAgentsObservationSchemaElement LocationSchema = ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema, 100.0f);
    FLearningAgentsObservationSchemaElement DirectionSchema = ULearningAgentsObservations::SpecifyDirectionObservation(InObservationSchema);

    TMap<FName, FLearningAgentsObservationSchemaElement> CharacterMap;
    CharacterMap.Add(TEXT("Location"), LocationSchema);
    CharacterMap.Add(TEXT("Direction"), DirectionSchema);
    FLearningAgentsObservationSchemaElement CharacterStruct = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, CharacterMap);

    // Enemy observation
    FLearningAgentsObservationSchemaElement EnemySchema = ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema, 100.0f);

    // Combined
    TMap<FName, FLearningAgentsObservationSchemaElement> CombinedMap;
    CombinedMap.Add(TEXT("Character"), CharacterStruct);
    CombinedMap.Add(TEXT("Enemy"), EnemySchema);
    OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, CombinedMap);
}

void ULearningAgentsInteractorPlus::GatherAgentObservation_Implementation(FLearningAgentsObservationObjectElement& OutObservationObjectElement, ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
    AAI_Blue_Plus* Agent = Cast<AAI_Blue_Plus>(GetAgent(AgentId));
    if (!Agent) return;

    const FVector Location = Agent->GetActorLocation();
    const FTransform Transform = Agent->GetActorTransform();
    const FVector Direction = Transform.GetRotation().GetForwardVector();
    const FVector EnemyLoc = Agent->NearestActorLocation; //Nearest enemy loc

    const FLearningAgentsObservationObjectElement LocationObs = ULearningAgentsObservations::MakeLocationObservation(InObservationObject, Location);
    const FLearningAgentsObservationObjectElement DirectionObs = ULearningAgentsObservations::MakeDirectionObservation(InObservationObject, Direction);

    TMap<FName, FLearningAgentsObservationObjectElement> CharacterMap;
    CharacterMap.Add(TEXT("Location"), LocationObs);
    CharacterMap.Add(TEXT("Direction"), DirectionObs);
    const FLearningAgentsObservationObjectElement CharacterStruct = ULearningAgentsObservations::MakeStructObservation(InObservationObject, CharacterMap);

    const FLearningAgentsObservationObjectElement EnemyObs = ULearningAgentsObservations::MakeLocationObservation(InObservationObject, EnemyLoc);

    TMap<FName, FLearningAgentsObservationObjectElement> CombinedMap;
    CombinedMap.Add(TEXT("Character"), CharacterStruct);
    CombinedMap.Add(TEXT("Enemy"), EnemyObs);

    OutObservationObjectElement = ULearningAgentsObservations::MakeStructObservation(InObservationObject, CombinedMap);
}

void ULearningAgentsInteractorPlus::SpecifyAgentAction_Implementation(FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
    const auto X_Schema = ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 10.0f);
    const auto Y_Schema = ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 10.0f);

    TMap<FName, FLearningAgentsActionSchemaElement> ActionMap;
    ActionMap.Add(TEXT("X_Input"), X_Schema);
    ActionMap.Add(TEXT("Y_Input"), Y_Schema);
    OutActionSchemaElement = ULearningAgentsActions::SpecifyStructAction(InActionSchema, ActionMap);
}

void ULearningAgentsInteractorPlus::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject, const FLearningAgentsActionObjectElement& InActionObjectElement, const int32 AgentId)
{
    AAI_Blue_Plus* Agent = Cast<AAI_Blue_Plus>(GetAgent(AgentId));
    if (!Agent) return;

    TMap<FName, FLearningAgentsActionObjectElement> ActionFields;
    ULearningAgentsActions::GetStructAction(ActionFields, InActionObject, InActionObjectElement);

    float X = 0.f;
    float Y = 0.f;

    ULearningAgentsActions::GetFloatAction(X, InActionObject, ActionFields["X_Input"]);
    ULearningAgentsActions::GetFloatAction(Y, InActionObject, ActionFields["Y_Input"]);

    Agent->MoveToTarget(Agent->GetActorLocation() + FVector(X, Y, 0.f));
}