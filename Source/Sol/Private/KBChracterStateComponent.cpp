// Fill out your copyright notice in the Description page of Project Settings.


#include "KBChracterStateComponent.h"
#include"KBGameInstance.h"

// Sets default values for this component's properties
UKBChracterStateComponent::UKBChracterStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UKBChracterStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKBChracterStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKBChracterStateComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UKBChracterStateComponent::SetNewLevel(int32 NewLevel)
{
	auto KBGameInstance = Cast<UKBGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	
	EROCHECK(nullptr != KBGameInstance);
	UE_LOG(Sol, Warning, TEXT("SetNewLevel A"));
	CurrentStatData = KBGameInstance->GetKBCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		UE_LOG(Sol, Warning, TEXT("SetNewLevel B"));
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
		UE_LOG(Sol, Warning, TEXT("SetNewLevel %f"), CurrentHP);
	}
	else
	{
		UE_LOG(Sol, Warning, TEXT("Level (%d) data dosen't exist."), NewLevel);
	}
	
}

void UKBChracterStateComponent::SetDamage(float NewDamage) 
{
	EROCHECK(nullptr != CurrentStatData);
	UE_LOG(Sol, Warning, TEXT("SetDamage %f"), FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UKBChracterStateComponent::SetHP(float NewHP) 
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

int32 UKBChracterStateComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}

float UKBChracterStateComponent::GetAttack()
{
	EROCHECK(nullptr != CurrentStatData,0.0f);
	return CurrentStatData->Attack;
}

float UKBChracterStateComponent::GetHPRatio()
{
	EROCHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxHP<KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP/CurrentStatData->MaxHP);
}

