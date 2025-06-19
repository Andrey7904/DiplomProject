
#include "AI_Blue_Plus.h"
#include "Kismet/GameplayStatics.h"
#include "LearningAgentsManager.h"
#include "AIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "NavigationSystem.h"

AAI_Blue_Plus::AAI_Blue_Plus()
{
	PrimaryActorTick.bCanEverTick = true;

	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

}

void AAI_Blue_Plus::BeginPlay()
{
	Super::BeginPlay();
	
	FindAndRegisterManager();
	if (AIPerception)
	{
		AIPerception->OnPerceptionUpdated.AddDynamic(this, &AAI_Blue_Plus::OnPerceptionUpdated);
	}
}

void AAI_Blue_Plus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MinDistanceSimple();
	RotateTowardsEnemy(Enemyseen_temp);
}

void AAI_Blue_Plus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAI_Blue_Plus::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	float Distance = 0.0f;
	bool bError = false;
	TArray<AActor*> LocalEnemySeenTemp;

	MinDistanceSimple(Distance, bError, LocalEnemySeenTemp);

	Enemyseen_temp = LocalEnemySeenTemp;
}

void AAI_Blue_Plus::FindAndRegisterManager()
{
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("LearningAgentsManagerBlue"), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        if (ULearningAgentsManager* Manager = Cast<ULearningAgentsManager>(Actor->GetComponentByClass(ULearningAgentsManager::StaticClass())))
        {
            if (Manager)
            {
                Manager->AddAgent(this);
                bFoundManager = true;
                break;
            }
        }
    }
}

void AAI_Blue_Plus::MinDistanceSimple()
{
    EnemyAlive.Empty();

    for (AActor* Actor : Enemy)
    {
        if (!IsValid(Actor)) continue;

        APlayer_Parent* Player = Cast<APlayer_Parent>(Actor);
        if (Player && !Player->IsDead())
        {
            EnemyAlive.Add(Player);
        }
    }

    Enemyseen_temp = EnemyAlive;

    AActor* Nearest = nullptr;
    float MinDist = TNumericLimits<float>::Max();
    FVector MyLocation = GetActorLocation();

    for (AActor* EnemyActor : EnemyAlive)
    {
        if (!IsValid(EnemyActor)) continue;

        float Dist = FVector::Dist(MyLocation, EnemyActor->GetActorLocation());
        if (Dist < MinDist)
        {
            MinDist = Dist;
            Nearest = EnemyActor;
        }
    }

    if (Nearest)
    {
        NearestActorLocation = Nearest->GetActorLocation();
    }
}

void AAI_Blue_Plus::RotateTowardsEnemy(const TArray<AActor*>& SeenEnemies)
{
    if (SeenEnemies.Num() == 0) return;

    FVector MyLocation = GetActorLocation();
    FVector MyForward = GetActorForwardVector();

    float MinAngle = 180.f;
    int32 ClosestIndex = -1;

    for (int32 i = 0; i < SeenEnemies.Num(); ++i)
    {
        AActor* Enemy = SeenEnemies[i];
        if (!IsValid(Enemy)) continue;

        FVector EnemyLocation = Enemy->GetActorLocation();
        FVector Direction = (EnemyLocation - MyLocation).GetSafeNormal();

        float Dot = FVector::DotProduct(MyForward, Direction);
        float Angle = FMath::Acos(Dot) * (180.0f / PI);

        if (Angle < MinAngle)
        {
            MinAngle = Angle;
            ClosestIndex = i;
        }
    }

    if (ClosestIndex != -1)
    {
        AActor* ClosestEnemy = SeenEnemies[ClosestIndex];
        if (IsValid(ClosestEnemy))
        {
            FVector Direction = (ClosestEnemy->GetActorLocation() - MyLocation).GetSafeNormal();
            FRotator NewRotation = Direction.Rotation();
            SetActorRotation(NewRotation);
        }
    }
}

void AAI_Blue_Plus::AIShoot()
{}

void AAI_Blue_Plus::MoveToTarget(FVector MoveLocation)
{
    USkeletalMeshComponent* MeshComp = FindComponentByClass<USkeletalMeshComponent>();
    if (!MeshComp) return;

    static ConstructorHelpers::FClassFinder<UAnimInstance> CombatAnimBP(TEXT("/Game/Path/To/ADP_AI_Manny"));
    if (CombatAnimBP.Succeeded())
    {
        MeshComp->SetAnimInstanceClass(CombatAnimBP.Class);
    }

    AAIController* AICont = Cast<AAIController>(GetController());
    if (!AICont) return;

    EPathFollowingRequestResult::Type Result = AICont->MoveToLocation(MoveLocation, 5.0f);

    if (!Result)
    {
        UE_LOG(LogTemp, Warning, TEXT("MoveTo failed"));

        static ConstructorHelpers::FClassFinder<UAnimInstance> IdleAnimBP(TEXT("/Game/CharacterAssets/Players/AI/Animations/ABP_AI_Manny_MoveToTarget")); 
        if (IdleAnimBP.Succeeded())
        {
            MeshComp->SetAnimInstanceClass(IdleAnimBP.Class);
        }
    }
}