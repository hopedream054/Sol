// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "GameFramework/PlayerState.h"
#include "KBPlayerState.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate)
/**
 * 
 */
UCLASS()
class SOL_API AKBPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AKBPlayerState();
	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	int32 GetGameHighScore() const;
	int32 GetCharacterIndex() const;

	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	void AddGameScore();

	void InitPlayerDate();
	void SavePlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;
	FString SaveSlotName;

protected:
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 GameHighScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	int32 Exp;

	UPROPERTY(Transient)
	int32 CharacterIndex;
private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FABCharacterData* CurrentStatData;
};
