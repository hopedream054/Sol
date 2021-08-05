// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "Animation/AnimInstance.h"
#include "KBAnimInstance.generated.h"



DECLARE_MULTICAST_DELEGATE(FOnAttackHitCHeckDelegate);
/**
 * 
 */
UCLASS()
class SOL_API UKBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UKBAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
private:
	UFUNCTION()
	void AnimNotify_shot();
private:
	//AllowPrivateAccess  : �̰� ���ָ� private �Լ��� �������Ʈ���� ��밡��.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInaAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

public:
	FOnAttackHitCHeckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }
};
