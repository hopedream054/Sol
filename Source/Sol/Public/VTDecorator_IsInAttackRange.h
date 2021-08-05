// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "BehaviorTree/BTDecorator.h"
#include "VTDecorator_IsInAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API UVTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UVTDecorator_IsInAttackRange();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
