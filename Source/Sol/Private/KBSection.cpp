// Fill out your copyright notice in the Description page of Project Settings.


#include "KBSection.h"
#include"KBCharacter.h"
#include"KItemBox.h"
#include"DrawDebugHelpers.h"
#include"KBPlayerController.h"
#include"KBGameMode.h"

// Sets default values
AKBSection::AKBSection()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	FString AssetPath = TEXT("StaticMesh'/Game/Meshes/Floor.Floor'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
	if (SM_SQUARE.Succeeded())
	{
		Mesh->SetStaticMesh(SM_SQUARE.Object);
		Mesh->SetRelativeScale3D(FVector(5.0f, 5.0f, 1.0f));
	}
	else
	{
		UE_LOG(Sol, Error, TEXT("Failed to load staticmesh asset. : %s"), *AssetPath);
	}

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 100.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Trigger->SetCollisionProfileName(TEXT("KBTrigger"));

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AKBSection::OnTriggerBeginOverlap);

	FString GateAssetPath = TEXT("StaticMesh'/Game/Meshes/Gate.Gate'");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
	if (!SM_GATE.Succeeded())
	{
		UE_LOG(Sol,Error, TEXT("Failed to load static Mesh asset. : %s"), *GateAssetPath);
	}
	

	static FName GateSockets[] = { {TEXT("X")}, {TEXT("-X")} ,{TEXT("Y")} ,{TEXT("-Y")} };
	for (FName GateSocket : GateSockets)
	{
		EROCHECK(Mesh->DoesSocketExist(GateSocket));
		UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
		NewGate->SetStaticMesh(SM_GATE.Object);
		NewGate->SetupAttachment(RootComponent, GateSocket);
		NewGate->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
		NewGate->SetRelativeScale3D(FVector(1.0f, 1.0f, 10.0f));
		GateMeshes.Add(NewGate);

		UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(
			*GateSocket.ToString().Append(TEXT("Trigger")));
		NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
		NewGateTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
		NewGateTrigger->SetCollisionProfileName(TEXT("KBTrigger"));
		GateTriggers.Add(NewGateTrigger);

		NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AKBSection::OnGateTriggerBeginOverlap);
		NewGateTrigger->ComponentTags.Add(GateSocket);//���ϸ����� ������Ʈ�� �±׸� �����صд�.
	}

	bNoBattle = false;
	EnemySpawnTime = 2.0f;
	ItemBoxSpawnTime = 5.0f;

}

//������ �۾����� ������ ������ �Ӽ��̳� Ʈ������ ������ ���� �� �� �� �Լ��� ����ȴ�.
void AKBSection::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);

}

// Called when the game starts or when spawned
void AKBSection::BeginPlay()
{
	Super::BeginPlay();

	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);

}

void AKBSection::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ESectionState::READY:
	{
		Trigger->SetCollisionProfileName(TEXT("KBTrigger"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			UE_LOG(Sol, Error, TEXT("READ"));
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(true);
		UE_LOG(Sol, Error, TEXT("READY"));
		break;
	}
	case ESectionState::BATTLE:
	{
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}
		OperateGates(false);

		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle, 
			FTimerDelegate::CreateUObject(this, &AKBSection::OnNPCSpawn), EnemySpawnTime, false);

		GetWorld()->GetTimerManager().SetTimer(SpawnItemBoxTimerHandle,
			FTimerDelegate::CreateLambda([this]()->void
		{
			FVector2D RandXY = FMath::RandPointInCircle(600.0f);
			GetWorld()->SpawnActor<AKItemBox>(GetActorLocation() + FVector(RandXY, 30.0f), FRotator::ZeroRotator);
		}
			),ItemBoxSpawnTime,false);
		

		UE_LOG(Sol, Error, TEXT("BATTLE"));
		break;
	}
	case ESectionState::COMPLETE:
	{

		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* GateTrigger : GateTriggers)
		{
			GateTrigger->SetCollisionProfileName(TEXT("KBTrigger"));
		}
		OperateGates(true);
		UE_LOG(Sol, Error, TEXT("COMPLETE"));
		break;
	}
	}

	CurrentState = NewState;
}

void AKBSection::OperateGates(bool bOpen)
{
	for (UStaticMeshComponent* Gate : GateMeshes)
	{
		Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, 0.0f, -90.0f) : FRotator::ZeroRotator);
	}
}

void AKBSection::OnNPCSpawn()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnNPCTimerHandle);
	auto KeyNPC = GetWorld()->SpawnActor<AKBCharacter>(
		GetActorLocation() + FVector::UpVector*88.0f, FRotator::ZeroRotator);
	if (nullptr != KeyNPC)
	{
		KeyNPC->OnDestroyed.AddDynamic(this, &AKBSection::OnKeyNPCDestroyed);
	}
}

void AKBSection::OnKeyNPCDestroyed(AActor * DestroyedActor)
{
	auto KBCharacter = Cast<AKBCharacter>(DestroyedActor);
	EROCHECK(nullptr != KBCharacter);

	auto KBPlayerController = Cast<AKBPlayerController>(KBCharacter->LastHitBy);
	EROCHECK(nullptr != KBPlayerController);

	auto KBGameMode = Cast<AKBGameMode>(GetWorld()->GetAuthGameMode());
	EROCHECK(nullptr != KBGameMode);
	KBGameMode->AddScore(KBPlayerController);

	SetState(ESectionState::COMPLETE);
}

void AKBSection::OnTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(Sol, Error, TEXT("BEGIN TRIGGER"));
	if (CurrentState == ESectionState::READY)
	{
		
		SetState(ESectionState::BATTLE);
	}
}

void AKBSection::OnGateTriggerBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	EROCHECK(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString());//��� �� �ǹ̴� ����.

	if (!Mesh->DoesSocketExist(ComponentTag))
	{
		UE_LOG(Sol, Warning, TEXT("Miss Socket : %s !!!"), *SocketName.ToString());
		return;
	}

	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	TArray < FOverlapResult > OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults, NewLocation, FQuat::Identity,
		ObjectQueryParam, FCollisionShape::MakeSphere(775.0f), CollisionQueryParam);

	DrawDebugSphere(GetWorld(), NewLocation, 775.0f, 16, FColor::Green, false, 2.2f);

	
	if (!bResult)
	{
		auto NewSection = GetWorld()->SpawnActor<AKBSection>(NewLocation*2- GetActorLocation(), FRotator::ZeroRotator);
		UE_LOG(Sol, Warning, TEXT("CCCCCCCCCCCCCCCCCC"));
	}
	else
	{
		UE_LOG(Sol, Warning, TEXT("New section area is not empty."));
		for (auto Resu : OverlapResults)
		{
			UE_LOG(Sol, Warning, TEXT("wath : %s"), *Resu.GetActor()->GetFName().ToString());
		}
	}
}

// Called every frame
void AKBSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

