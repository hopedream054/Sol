// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "GameFramework/GameModeBase.h"
#include "KBGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API AKBGameMode : public AGameModeBase
{
	GENERATED_BODY()

	AKBGameMode();
public:
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void AddScore(class AKBPlayerController *ScoredPlayer);

private:
	UPROPERTY()
	class AKBGameState* KBGameState;

	UPROPERTY()
	int32 ScoreToClear;
};
