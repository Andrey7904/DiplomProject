
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ThirdPersonCharacter_plus.generated.h"

UCLASS()
class DIPLOMPROJECT_API AThirdPersonCharacter_plus : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AThirdPersonCharacter_plus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//Health - damage system
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	float GetHealth(); // Реалізується в BP

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetHealth(float NewHealth);

	UFUNCTION()
	void HandleDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Health;

	// Shooting system
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Firing;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 AmmoAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 MagsAmount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool Reloading;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Shoot;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Shoot();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AmmoCounter();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void MagsCounter();

	void StartShooting();
	void ShootingCycle();
	void StopShooting();
	FTimerHandle ShootingTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;
	
	//weapon connectors
	UFUNCTION(BlueprintCallable)
	void ShootPlus();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	UAnimationAsset* WeaponFireAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Weapon;

	// Getter для BP
	FORCEINLINE USkeletalMeshComponent* GetWeapon() const { return Weapon; }

	//Counters for reload
	UFUNCTION(BlueprintCallable)
	void AmmoCounterPlus();

	UFUNCTION(BlueprintCallable)
	void MagsCounterPlus();

	//UPROPERTY(BlueprintReadWrite, Category = "UI")
	//class UWB_HUD_C* Widget_Hud;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	class UTextBlock* Ammo;

	// Crouching
	UFUNCTION(BlueprintCallable)
	void ToggleCrouch();

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	bool bCrouching;

	//Aiming
	UFUNCTION(BlueprintCallable)
	void StartAiming();

	UFUNCTION(BlueprintCallable)
	void StopAiming();

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool bIsAiming;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float AimBoomLength = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float DefaultBoomLength = 300.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* AimAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* CrouchAction;



};