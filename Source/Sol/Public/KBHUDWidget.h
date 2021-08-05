// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "Blueprint/UserWidget.h"
#include "KBHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API UKBHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindCharacterStat(class UKBChracterStateComponent* CharacterStat);
	void BindPlayerState(class AKBPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UPdatePlayerState();

private:
	TWeakObjectPtr<class UKBChracterStateComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AKBPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* PlayerName;

	UPROPERTY()
	class UTextBlock* PlayerLevel;

	UPROPERTY()
	class UTextBlock* CurrentScore;

	UPROPERTY()
	class UTextBlock* HighScore;
};
