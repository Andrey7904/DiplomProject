
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI_Blue_Plus.generated.h"

UCLASS()
class DIPLOMPROJECT_API AAI_Blue_Plus : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Blue_Plus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    UFUNCTION()
    void AIShoot();

    UFUNCTION(BlueprintCallable)
    void MoveToTarget(FVector MoveLocation);

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    void FindAndRegisterManager();
    void MinDistanceSimple();
    void RotateTowardsEnemy(const TArray<AActor*>& SeenEnemies);

public:
    UPROPERTY()
    bool bFoundManager = false;

    UPROPERTY()
    TArray<AActor*> Enemy;

    UPROPERTY()
    TArray<AActor*> EnemyAlive;

    UPROPERTY()
    TArray<AActor*> Enemyseen_temp;

    FVector NearestActorLocation;

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

    UFUNCTION(BlueprintCallable)
    void MinDistanceSimple(float& OutDistance, bool& bError, TArray<AActor*>& OutEnemySeenTemp);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    class UAIPerceptionComponent* AIPerception;
};