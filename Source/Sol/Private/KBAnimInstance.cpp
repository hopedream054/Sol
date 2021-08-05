// Fill out your copyright notice in the Description page of Project Settings.


#include "KBAnimInstance.h"

UKBAnimInstance::UKBAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInaAir = false;
	IsDead = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Attack_Montage(TEXT(
		"AnimMontage'/Game/Player/Montage/PlayerShoot.PlayerShoot'"));
	if (Attack_Montage.Succeeded())
	{
		AttackMontage = Attack_Montage.Object;
	}
	PlayAttackMontage();
}

void UKBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto Pawn = TryGetPawnOwner();

	if (!::IsValid(Pawn)) return;
	
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		//UE_LOG(Sol, Warning, TEXT("spedd %2f \n"),CurrentPawnSpeed);
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInaAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UKBAnimInstance::PlayAttackMontage()
{
	EROCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}

void UKBAnimInstance::AnimNotify_shot()
{
	UE_LOG(Sol, Warning, TEXT("attack \n"));
	OnAttackHitCheck.Broadcast();
}
