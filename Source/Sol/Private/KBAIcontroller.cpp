// Fill out your copyright notice in the Description page of Project Settings.


#include "KBAIcontroller.h"
#include"NavigationSystem.h"
#include"Blueprint/AIBlueprintHelperLibrary.h" // 심플 SimpleMoveToLocation 이거 사용하려고 씀
#include"BehaviorTree/BehaviorTree.h"
#include"BehaviorTree/BlackboardData.h"
#include"BehaviorTree/BlackboardComponent.h"

const FName AKBAIcontroller::HomePosKey(TEXT("HomePos"));
const FName AKBAIcontroller::PatrolPosKey(TEXT("PatrolPos"));
const FName AKBAIcontroller::TargetKey(TEXT("Target"));

AKBAIcontroller::AKBAIcontroller()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("BlackboardData'/Game/AI/BB_KBCharacter.BB_KBCharacter'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("BehaviorTree'/Game/AI/BT_KBCharacter.BT_KBCharacter'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}
void AKBAIcontroller::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	
}

void AKBAIcontroller::RunAI()
{
	
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(Sol, Warning, TEXT("AIController couldn't run begavior tree!"));
		}
	}
}

void AKBAIcontroller::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

//void AKBAIcontroller::OnUnPossess()
//{
//	Super::OnUnPossess();
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
//}
//
//void AKBAIcontroller::OnRepeatTimer()
//{
//	auto CurrentPawn = GetPawn();
//	EROCHECK(nullptr != CurrentPawn);
//
//	UNavigationSystemV1*  NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
//	if (nullptr == NavSystem) return;
//
//	FNavLocation NextLocation;
//	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
//	{
//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
//		UE_LOG(Sol, Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
//	}
//}
