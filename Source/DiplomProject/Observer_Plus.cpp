
#include "Observer_Plus.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "DrawDebugHelpers.h"
#include "AI_Blue_Plus.h"

// Sets default values
AObserver_Plus::AObserver_Plus()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObserver_Plus::BeginPlay()
{
	Super::BeginPlay();
	
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }
    }
}

// Called every frame
void AObserver_Plus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AObserver_Plus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AObserver_Plus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (SelectAction)
        {
            Input->BindAction(SelectAction, ETriggerEvent::Started, this, &AObserver_Plus::HandleSelect);
        }

        if (MoveCommandAction)
        {
            Input->BindAction(MoveCommandAction, ETriggerEvent::Triggered, this, &AObserver_Plus::HandleMoveCommand);
        }
    }
}

void AObserver_Plus::HandleSelect(const FInputActionInstance& Instance)
{
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    FHitResult HitResult;
    if (PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, HitResult))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor && HitActor->IsA<AAI_Blue_Plus>())
        {
            SelectedActor = HitActor;

            USceneComponent* Root = HitActor->GetRootComponent();
            if (Root)
            {
                Root->SetVisibility(false, false);
            }
        }
    }
}

void AObserver_Plus::HandleMoveCommand(const FInputActionInstance& Instance)
{
    if (!SelectedActor) return;

    // Отримати координати кліку миші
    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    FHitResult Hit;
    if (PC->GetHitResultUnderCursorByChannel(TraceTypeQuery3, true, Hit))
    {
        FVector TargetLocation = Hit.Location;

        AAI_Blue_Plus* BlueUnit = Cast<AAI_Blue_Plus>(SelectedActor);
        if (BlueUnit)
        {
            BlueUnit->MoveToTarget(TargetLocation);
        }
    }
}