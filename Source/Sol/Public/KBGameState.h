// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "GameFramework/GameStateBase.h"
#include "KBGameState.generated.h"

/**
 *
 */
UCLASS()
class SOL_API AKBGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AKBGameState();

public:
	int32 GetTotalGameScore() const;
	void AddGameScore();
	void SetGameCleared();
	bool IsGameCleared() const;

private:
	UPROPERTY(Transient)
	int32 TotalGameScore;

	UPROPERTY(Transient)
	bool bGameCleared;
};
