// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "AIController.h"
#include "KBAIcontroller.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API AKBAIcontroller : public AAIController
{
	GENERATED_BODY()
public:
	AKBAIcontroller();
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	void RunAI();
	void StopAI();

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
private:
	//void OnRepeatTimer();
	//
	//FTimerHandle RepeatTimerHandle;
	//float RepeatInterval;

	UPROPERTY()
	class UBehaviorTree* BTAsset;
	UPROPERTY()
	class UBlackboardData* BBAsset;
};
