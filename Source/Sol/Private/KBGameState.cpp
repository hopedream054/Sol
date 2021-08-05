// Fill out your copyright notice in the Description page of Project Settings.


#include "KBGameState.h"

AKBGameState::AKBGameState()
{
	TotalGameScore = 0;
	bGameCleared = false;
}

int32 AKBGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void AKBGameState::AddGameScore()
{
	TotalGameScore++;
}

void AKBGameState::SetGameCleared()
{
	bGameCleared = true;
}

bool AKBGameState::IsGameCleared() const
{
	return bGameCleared;
}
