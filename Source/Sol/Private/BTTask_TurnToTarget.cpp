// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include"KBAIcontroller.h"
#include"KBCharacter.h"
#include"BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto KBCharacter = Cast<AKBCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == KBCharacter)
	{
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<AKBCharacter>(OwnerComp.GetBlackboardComponent()
		->GetValueAsObject(AKBAIcontroller::TargetKey));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	FVector LookVector = Target->GetActorLocation() - KBCharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	KBCharacter->SetActorRotation(FMath::RInterpTo(KBCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
