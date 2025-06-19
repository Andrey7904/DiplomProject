#include "ThirdPersonCharacter_plus.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"




// Sets default values
AThirdPersonCharacter_plus::AThirdPersonCharacter_plus()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Health = 2.0f;
    MagsAmount = 5;
    AmmoAmount = 30;

}

// Called when the game starts or when spawned
void AThirdPersonCharacter_plus::BeginPlay()
{
    Super::BeginPlay();

    //UKismetSystemLibrary::PrintString(this, FString("Health: "), true, true, FLinearColor::Green, 3.0f);
    //UE_LOG(LogTemp, Warning, TEXT("BeginPlay called on %s"), *GetName());

    float CurrentHealth = GetHealth();

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }
    }

    OnTakeAnyDamage.AddDynamic(this, &AThirdPersonCharacter_plus::HandleDamage);
}

// Called every frame
void AThirdPersonCharacter_plus::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPersonCharacter_plus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(IA_Shoot, ETriggerEvent::Started, this, &AThirdPersonCharacter_plus::StartShooting);
        EnhancedInput->BindAction(IA_Shoot, ETriggerEvent::Completed, this, &AThirdPersonCharacter_plus::StopShooting);
        EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter_plus::ToggleCrouch);
        EnhancedInput->BindAction(AimAction, ETriggerEvent::Started, this, &AThirdPersonCharacter_plus::StartAiming);
        EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &AThirdPersonCharacter_plus::StopAiming);

    }
}

void AThirdPersonCharacter_plus::HandleDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
    AController* InstigatedBy, AActor* DamageCauser)
{

    if (Health > 0.0f)
    {
        Health = Health - Damage;
        UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Damage taken: %.2f"), Damage));
        UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("New health: %.2f"), Health));

        if (Health <= 0.0f)
        {
            GetCharacterMovement()->DisableMovement();
            APlayerController* PlayerController = Cast<APlayerController>(GetController());
            if (PlayerController)
            {
                DisableInput(PlayerController);
            }

            GetMesh()->SetSimulatePhysics(true);
            GetMesh()->SetPhysicsBlendWeight(1.0f);
            GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        }
    }

}

void AThirdPersonCharacter_plus::StartShooting()
{
    Firing = true;
    ShootingCycle();

}

void AThirdPersonCharacter_plus::ShootingCycle()
{
    if (!Firing) return;
    if (Reloading) return;

    if (AmmoAmount > 0)
    {
        //Shoot();
        ShootPlus();
        AmmoCounter();
        GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &AThirdPersonCharacter_plus::ShootingCycle, 0.1f, false);
    }
    else if (MagsAmount > 0)
    {
        UKismetSystemLibrary::PrintString(this, TEXT("RELOADING"));

        GetWorld()->GetTimerManager().SetTimer(ShootingTimerHandle, this, &AThirdPersonCharacter_plus::MagsCounter, 3.0f, false);
        Reloading = true;
    }
}

void AThirdPersonCharacter_plus::StopShooting()
{
    Firing = false;
}

void AThirdPersonCharacter_plus::ShootPlus()
{
    if (!Weapon) return;

    if (WeaponFireAnim)
    {
        Weapon->PlayAnimation(WeaponFireAnim, false);
    }
    else {
        UKismetSystemLibrary::PrintString(this, TEXT("ERROR SHOOTING"));
    }

    FVector CamStart;
    FVector CamEnd;
    UWorld* World = GetWorld();
    if (World)
    {

        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        APlayerCameraManager* CameraManager = PlayerController ? PlayerController->PlayerCameraManager : nullptr;


        //APlayerCameraManager* CameraManager = GetPlayerCameraManager(World);
        if (CameraManager)
        {
            CamStart = CameraManager->GetCameraLocation();
            FRotator CameraRotation = CameraManager->GetCameraRotation();

            //FVector ForwardVector = CameraRotation.Vector();
            CamEnd = CamStart + (CameraRotation.Vector() * 30000.0f);
        }
    }

    FHitResult SightHit;
    FCollisionQueryParams CamTraceParams(SCENE_QUERY_STAT(AISight), true, this);
    CamTraceParams.bReturnPhysicalMaterial = false;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bSawTarget = GetWorld()->LineTraceSingleByChannel(
        SightHit,
        CamStart,
        CamEnd,
        ECC_GameTraceChannel1, //HitIdentify
        CamTraceParams
    );

    FVector ImpactPoint = SightHit.ImpactPoint;

    if (bSawTarget)
    {
        // Постріл з дула гвинтівки
        FVector FireStart = Weapon->GetSocketLocation("MuzzleFlash");


        FHitResult FireHit;
        FCollisionQueryParams FireTraceParams(SCENE_QUERY_STAT(AIShoot), true, this);
        FireTraceParams.AddIgnoredActor(this);

        bool bDidHit = GetWorld()->LineTraceSingleByChannel(
            FireHit,
            FireStart,
            ImpactPoint,
            ECC_GameTraceChannel1, //HitIdentify
            FireTraceParams
        );
        DrawDebugLine(GetWorld(), FireStart, ImpactPoint, FColor::Red, false, 3.0f, 0, 2.0f);

        if (bDidHit)
        {
            AActor* HitActor = FireHit.GetActor();
            if (HitActor)
            {
                UGameplayStatics::ApplyDamage(
                    HitActor,
                    1.0f,
                    GetController(),
                    this,
                    nullptr
                );
            }
        }
    }
}

APlayerCameraManager* GetPlayerCameraManager(UWorld* World)
{
    // Отримуємо контролер гравця
    APlayerController* PlayerController = World->GetFirstPlayerController();

    if (PlayerController)
    {
        return PlayerController->PlayerCameraManager;
    }

    return nullptr;
}

void AThirdPersonCharacter_plus::AmmoCounterPlus()
{
    AmmoAmount = FMath::Clamp(AmmoAmount - 1, 0, 30);

    // Оновити hud
    if (Ammo)
    {
        FText AmmoText = FText::AsNumber(AmmoAmount);
        //Ammo->SetText(AmmoText);
    }
}

void AThirdPersonCharacter_plus::MagsCounterPlus()
{
    // Зменшити кількість магазинів на 1
    MagsAmount = FMath::Clamp(MagsAmount - 1, 0, 7);

    // Оновити HUD
    if (Ammo)
    {
        FText MagsText = FText::AsNumber(AmmoAmount);
        //Ammo->SetText(MagsText);
    }

    AmmoAmount = 30;
}

void AThirdPersonCharacter_plus::ToggleCrouch()
{
    if (!GetCharacterMovement()) return;

    bCrouching = !bCrouching;

    if (bCrouching)
    {
        Crouch();
        GetCharacterMovement()->MaxWalkSpeed = 150.0f;
    }
    else
    {
        UnCrouch();
        GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    }
}

void AThirdPersonCharacter_plus::StartAiming()
{
    bIsAiming = true;
    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = AimBoomLength;
    }
}

void AThirdPersonCharacter_plus::StopAiming()
{
    bIsAiming = false;
    if (CameraBoom)
    {
        CameraBoom->TargetArmLength = DefaultBoomLength;
    }
}

