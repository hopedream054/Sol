// Fill out your copyright notice in the Description page of Project Settings.


#include "VTDecorator_IsInAttackRange.h"
#include"KBAIcontroller.h"
#include"KBCharacter.h"
#include"BehaviorTree/BlackboardComponent.h"

UVTDecorator_IsInAttackRange::UVTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UVTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ContorllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ContorllingPawn)
	{
		return false;
	}

	auto Target = Cast<AKBCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AKBAIcontroller::TargetKey));
	if (nullptr == Target)
	{
		return false;
	}
	bResult = (Target->GetDistanceTo(ContorllingPawn) <= 200.0f);
	return bResult;
}
