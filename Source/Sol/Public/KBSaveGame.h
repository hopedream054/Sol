// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "GameFramework/SaveGame.h"
#include "KBSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API UKBSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UKBSaveGame();

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Exp;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 HighScore;

	UPROPERTY()
	int32 CharacterIndex;
};
