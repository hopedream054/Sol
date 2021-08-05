// Fill out your copyright notice in the Description page of Project Settings.


#include "KBPlayerState.h"
#include "KBGameInstance.h"
#include"KBSaveGame.h"

AKBPlayerState::AKBPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 0;
	Exp = 0;
	CharacterIndex = 0;
	SaveSlotName = TEXT("Player1");
}

int32 AKBPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AKBPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

int32 AKBPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

int32 AKBPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}

float AKBPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	UE_LOG(Sol, Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

bool AKBPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
	{
		return false;
	}

	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
	return DidLevelUp;
}

void AKBPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

void AKBPlayerState::InitPlayerDate()
{
	auto KBSaveGame = Cast<UKBSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == KBSaveGame)
	{
		KBSaveGame = GetMutableDefault<UKBSaveGame>();
	}

	SetPlayerName(KBSaveGame->PlayerName);
	SetCharacterLevel(KBSaveGame->Level);
	
	GameScore = 0;
	GameHighScore = KBSaveGame->HighScore;
	Exp =KBSaveGame->Exp;
	CharacterIndex = KBSaveGame->CharacterIndex;
	SavePlayerData();
}

void AKBPlayerState::SavePlayerData()
{
	UKBSaveGame* NewPlayerData = NewObject<UKBSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		UE_LOG(Sol,Error,TEXT("SaveGame Error!"));
	}

}

void AKBPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto KBGameInstance = Cast<UKBGameInstance>(GetGameInstance());
	EROCHECK(nullptr != KBGameInstance);

	CurrentStatData = KBGameInstance->GetKBCharacterData(NewCharacterLevel);
	EROCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}
