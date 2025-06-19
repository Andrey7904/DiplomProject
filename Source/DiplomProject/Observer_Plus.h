
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Observer_Plus.generated.h"

UCLASS()
class DIPLOMPROJECT_API AObserver_Plus : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AObserver_Plus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* SelectAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveCommandAction;

	UPROPERTY(BlueprintReadOnly)
	AActor* SelectedActor;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void HandleSelect(const struct FInputActionInstance& Instance);

	UFUNCTION()
	void HandleMoveCommand(const struct FInputActionInstance& Instance);

	UPROPERTY(BlueprintReadWrite)
	AActor* SelectedActor;

};
