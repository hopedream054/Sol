// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "Blueprint/UserWidget.h"
#include "KBCharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API UKBCharacterWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	void BindCharacterStat(class UKBChracterStateComponent* NewCharacterStat);
protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
private:
	TWeakObjectPtr<class UKBChracterStateComponent> CurrentCharacterStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
