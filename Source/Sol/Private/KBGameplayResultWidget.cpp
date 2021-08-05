// Fill out your copyright notice in the Description page of Project Settings.


#include "KBGameplayResultWidget.h"
#include "Components/TextBlock.h"
#include"KBGameState.h"

void UKBGameplayResultWidget::BindGameState(class AKBGameState* GameState)
{
	EROCHECK(nullptr != GameState);
	CurrentGameState = GameState;
}

void UKBGameplayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	EROCHECK(CurrentGameState.IsValid());

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	EROCHECK(nullptr != Result);
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ?
		TEXT("Mission Complete") : TEXT("Mission Failed")));
	
	auto TotalScore=Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	EROCHECK(nullptr != TotalScore);
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}