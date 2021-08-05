// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include"KBAIcontroller.h"
#include "KBCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto KBCharacter = Cast<AKBCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == KBCharacter)
	{
		return EBTNodeResult::Failed;
	}

	KBCharacter->Attack();
	IsAttacking = true;
	KBCharacter->OnAttackEnd.AddLambda([this]()->void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);//태스크가 끝났다고 알려주기 위하여
	}
}
