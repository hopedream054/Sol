// Fill out your copyright notice in the Description page of Project Settings.


#include "KBPawn.h"

// Sets default values
AKBPawn::AKBPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPEINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	Capsule->SetCapsuleHalfHeight(90.0f);
	Capsule->SetCapsuleRadius(25.0f);
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-30.0f,0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> myBp(TEXT(
		"SkeletalMesh'/Game/ParagonMurdock/Characters/Heroes/Murdock/Meshes/Murdock.Murdock'"));
	if (myBp.Succeeded())
	{
		Mesh->SetSkeletalMesh(myBp.Object);
	}


	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> Player_Anim(TEXT(
		"AnimBlueprint'/Game/Player/Animinstance/GunnerAniBP.GunnerAniBP_C'"));
	if (Player_Anim.Succeeded())
	{
		Mesh->SetAnimInstanceClass(Player_Anim.Class);
	}
}

// Called when the game starts or when spawned
void AKBPawn::BeginPlay()
{
	Super::BeginPlay();

	/*Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, 
		TEXT("AnimSequence'/Game/ParagonMurdock/Characters/Heroes/Murdock/Animations/Idle_A_Loop.Idle_A_Loop'"));

	if (AnimAsset != nullptr)
	{
		Mesh->PlayAnimation(AnimAsset, true);
	}

	*/
}

// Called every frame
void AKBPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKBPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AKBPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AKBPawn::LeftRight);
}

void AKBPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AKBPawn::PossessedBy(AController * NewController)
{

}

void AKBPawn::UpDown(float NewAxisValue)
{

}

void AKBPawn::LeftRight(float NewAxisValue)
{

}


//138p