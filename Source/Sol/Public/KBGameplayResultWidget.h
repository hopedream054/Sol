// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "KBGameplayWidget.h"
#include "KBGameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API UKBGameplayResultWidget : public UKBGameplayWidget
{
	GENERATED_BODY()
public:
	void BindGameState(class AKBGameState* GameState);
protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class AKBGameState> CurrentGameState;
};
//607
