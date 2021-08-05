// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "GameFramework/Character.h"
#include "KBCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttack)
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate)

UCLASS()
class SOL_API AKBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKBCharacter();

	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	enum class EControlMode
	{
		d3Back,
		d3Top,
		NPC
	};



	EControlMode CurrentControlMode = EControlMode::d3Back;
	FVector DirectionToMove = FVector::ZeroVector;
	
	float ArmLengthTo = 0.0f; //
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;


	void SetControlMode(EControlMode ControlMode);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;
	virtual void PossessedBy(AController* NewController) override;

	bool CanSetWeapon();
	void SetWeapon(class AKWeaponGun* NewWeapon);
	void Attack();
	int32 GetExp() const;
public:

	

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AKWeaponGun* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class UKBChracterStateComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	FOnAttackEndDelegate OnAttackEnd;
private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void TurnRight(float NewAxisValue);

	void ViewChange();
	void AttackCheck();

	void OnAssetLoadCompleted();

	UFUNCTION()
	void OnAttackMonageEnded(UAnimMontage* Montage, bool bInterrupted);

	
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY()
	class UKBAnimInstance* KBAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float Deal;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	UPROPERTY()
	class AKBAIcontroller* KBAIController;

	UPROPERTY()
	class AKBPlayerController* KBPlayerController;


	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
	//https://docs.unrealengine.com/4.26/ko/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/SmartPointerLibrary/

	int32 AssetIndex = 0;

	FTimerHandle DeadTimerHandle = {};
};
