// Fill out your copyright notice in the Description page of Project Settings.


#include "KBCharacter.h"
#include"KBAnimInstance.h"
#include"KWeaponGun.h"
#include"KBChracterStateComponent.h"
#include"DrawDebugHelpers.h"
#include"Components/WidgetComponent.h"
#include"KBCharacterWidget.h"
#include"KBAIcontroller.h"
#include"KBCharacterSetting.h"
#include"KBGameInstance.h"
#include"KBPlayerController.h"
#include"KBPlayerState.h"
#include"KBHUDWidget.h"

// Sets default values
AKBCharacter::AKBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(Sol, Warning, TEXT("Create AKBCharacter"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPEINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UKBChracterStateComponent>(TEXT("Stat"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	AIControllerClass = AKBAIcontroller::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; // �����ɼ��� PlacedInWorldOrSpawned�� �����Ѵ�. �׷��� ���� �����ǰų� ���忡 ��ġ�Ǵ� �� ĳ���ʹ� AI�� ��Ʈ���� �ް� �ȴ�.


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> myBp(TEXT(
		"SkeletalMesh'/Game/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant.Revenant'"));
	if (myBp.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(myBp.Object);
	}


	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> Player_Anim(TEXT(
		"AnimBlueprint'/Game/Player/Animinstance/Player_AnimBP.Player_AnimBP_C'"));
	if (Player_Anim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Player_Anim.Class);
	}


	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("KCharacter"));
	

	//HP�� ����
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 230.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("WidgetBlueprint'/Game/UI/KBPUI.KBPUI_C'"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
	

	SetControlMode(EControlMode::d3Back); // �þ� ���


	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false; // ���ñ���
	
	//���� �Ÿ��� ũ��
	AttackRange = 100.0f;
	AttackRadius = 50.0f;
	Deal = 50.0f;

	AssetIndex = 4;

	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);

	DeadTimer = 5.0f;

	UE_LOG(Sol, Warning, TEXT("KBCharacter() end"));
}

// Called when the game starts or when spawned
void AKBCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		KBPlayerController = Cast<AKBPlayerController>(GetController());
		EROCHECK(nullptr != KBPlayerController);
	}
	else
	{
		KBAIController = Cast<AKBAIcontroller>(GetController());
		EROCHECK(nullptr != KBAIController);
	}

	auto DefaultSetting = GetDefault<UKBCharacterSetting>();

	if (bIsPlayer)
	{
		AssetIndex = 1;
		//auto KBPlayerState = Cast<AKBPlayerState>(PlayerState);

	}
	else
	{
		//AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
		AssetIndex = 1;
	}

	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto KBGameInstance = Cast<UKBGameInstance>(GetGameInstance());
	EROCHECK(nullptr != KBGameInstance);
	AssetStreamingHandle = KBGameInstance->StreambleManager.RequestAsyncLoad(
	CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AKBCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);

	//if (!IsPlayerControlled())
	//{
	//	auto DefaultSetting = GetDefault<UKBCharacterSetting>();
	//	int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	//	CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];
	//
	//	auto KBGameInstace = Cast<UKBGameInstance>(GetGameInstance());
	//	if (nullptr != KBGameInstace)
	//	{
	//		AssetStreamingHandle = KBGameInstace->StreambleManager.RequestAsyncLoad(CharacterAssetToLoad, 
	//			FStreamableDelegate::CreateUObject(this,&AKBCharacter::OnAssetLoadCompleted));
	//	}
	//}
}

void AKBCharacter::SetControlMode(EControlMode ControlMode)
{
	CurrentControlMode = ControlMode;
	switch (CurrentControlMode)
	{
	case EControlMode::d3Back: //3��Ī ���
		ArmLengthTo = 400.0f;
		SpringArm->TargetArmLength = 400.0f;
		SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::d3Top: //3��Ī ž��
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;

	case EControlMode::NPC :
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

// Called every frame
void AKBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	SpringArm->TargetArmLength = FMath::FInterpTo(
		SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::d3Back:
		SpringArm->SetRelativeRotation(FMath::RInterpTo(
			SpringArm->GetRelativeRotation(),ArmRotationTo, DeltaTime,ArmRotationSpeed));
	case EControlMode::d3Top:
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
			break;
		}
	}
}

// Called to bind functionality to input
void AKBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this,
		&AKBCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this,
		&AKBCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shot"), EInputEvent::IE_Pressed, this,
		&AKBCharacter::Attack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AKBCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AKBCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AKBCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("TurnRight"), this, &AKBCharacter::TurnRight);
}

void AKBCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	KBAnim = Cast<UKBAnimInstance>(GetMesh()->GetAnimInstance());
	EROCHECK(nullptr != KBAnim);

	KBAnim->OnMontageEnded.AddDynamic(this, &AKBCharacter::OnAttackMonageEnded);
	KBAnim->OnAttackHitCheck.AddUObject(this, &AKBCharacter::AttackCheck);

	CharacterStat->OnHPIsZero.AddLambda([this]()->void
	{
			UE_LOG(Sol, Warning, TEXT("OnHPIsZero"));
			KBAnim->SetDeadAnim();
			SetActorEnableCollision(false);
	});

	
	HPBarWidget->InitWidget(); //�̰ɷ� �ʱ�ȭ�� Ȯ�����ش�. GetUserWidgetObject �̳��� PostInitializeComponents()���� ȣ���ҋ� null�� ��ȯ�ؼ� �̰� �־��ذŴ�.
	auto CharacterWidget = Cast< UKBCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
	else
	{
		UE_LOG(Sol, Warning, TEXT("CharacterWidget == nullptr"));
	}
}

float AKBCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(Sol, Warning, TEXT("%s takes %f damage from %s"), *GetName(), FinalDamage, *DamageCauser->GetName());
	
	//if (FinalDamage > 0.0f)
	//{
	//	KBAnim->SetDeadAnim();
	//	SetActorEnableCollision(false);
	//}

	CharacterStat->SetDamage(FinalDamage);

	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto instigator = Cast<AKBPlayerController>(EventInstigator);
			EROCHECK(nullptr != instigator, 0.0f);
			instigator->NPCKill(this);
		}
	}
	return FinalDamage;
}

void AKBCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::d3Back);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

bool AKBCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void AKBCharacter::SetWeapon(AKWeaponGun * NewWeapon)
{
	EROCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);

	FName WeaponSocket(TEXT("gun_weapon"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);//�ַ� ��Ʈ��ũ �������� ������ ������ ����
		CurrentWeapon = NewWeapon;
	}
}

void AKBCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::d3Back:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::d3Top:
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

void AKBCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::d3Back:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::d3Top:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
	
	
}

void AKBCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::d3Back:
		AddControllerPitchInput(NewAxisValue*1.0f);
		break;
	case EControlMode::d3Top:
		break;
	}
	
}

void AKBCharacter::TurnRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::d3Back:
		AddControllerYawInput(NewAxisValue * 1.0f);
		break;
	case EControlMode::d3Top:
		break;
	}
	
}

void AKBCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
	case EControlMode::d3Back:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::d3Top);
		break;
	case EControlMode::d3Top:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::d3Back);
		break;
	}
}

void AKBCharacter::Attack()
{
	if (IsAttacking) return;


	KBAnim->PlayAttackMontage();
	IsAttacking = true;
}

void AKBCharacter::AttackCheck()
{
	//ECC_GameTraceChannel2
	UE_LOG(Sol, Warning, TEXT("AttackCheck Delegate \n"));
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector()*AttackRange,
		FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(AttackRadius),
		Params);


	FVector TraceVec = GetActorForwardVector()*AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CpasuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTrime = 2.0f;
	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
		CpasuleRot, DrawColor, false, DebugLifeTrime);
	

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			UE_LOG(Sol, Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			UE_LOG(Sol, Warning, TEXT("%s deals %f damage to %s"), *GetName(), CharacterStat->GetAttack(), *HitResult.Actor->GetName());
		}
	}
}

void AKBCharacter::OnAttackMonageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	EROCHECK(IsAttacking);
	IsAttacking = false;
	UE_LOG(Sol, Warning, TEXT("Attack End \n"));
	//297
	OnAttackEnd.Broadcast();
}




void Attack()
{
	
}

int32 AKBCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

void AKBCharacter::OnAssetLoadCompleted()
{
	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(CharacterAssetToLoad);
	EROCHECK(LoadedAssetPath.IsValid());
	
	GetMesh()->SetSkeletalMesh(LoadedAssetPath.Get());
	SetCharacterState(ECharacterState::READY);
}

void AKBCharacter::SetCharacterState(ECharacterState NewState)
{
	EROCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			DisableInput(KBPlayerController);

			KBPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			auto KBPlayerState = Cast<AKBPlayerState>(GetPlayerState());
			EROCHECK(nullptr != KBPlayerState);
			CharacterStat->SetNewLevel(KBPlayerState->GetCharacterLevel());
		}

		break;
	}
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);
		
		CharacterStat->OnHPIsZero.AddLambda([this]()->void
		{
			SetCharacterState(ECharacterState::DEAD);
		});
		auto CharacterWidget = Cast<UKBCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		EROCHECK(nullptr != CharacterWidget);
		CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::d3Top);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(KBPlayerController);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			KBAIController->RunAI();
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(false);
		GetMesh()->SetHiddenInGame(false);
		KBAnim->SetDeadAnim();

		if (bIsPlayer)
		{
			DisableInput(KBPlayerController);
		}
		else
		{
			KBAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]()-> void {
			if (bIsPlayer)
			{
				//KBPlayerController->RestartLevel();
				KBPlayerController->ShowResultUI();
			}
			else
			{
				Destroy();
			}
		}
		),DeadTimer,false);
		break;
	}
	}
}

ECharacterState AKBCharacter::GetCharacterState() const
{
	return CurrentState;
}