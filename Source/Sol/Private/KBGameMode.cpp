// Fill out your copyright notice in the Description page of Project Settings.

#include "KBGameMode.h"
#include "KBCharacter.h"
#include"KBPlayerController.h"
#include"KBPlayerState.h"
#include"KBGameState.h"

AKBGameMode::AKBGameMode()
{
	DefaultPawnClass = AKBCharacter::StaticClass();

	PlayerControllerClass = AKBPlayerController::StaticClass();
	PlayerStateClass = AKBPlayerState::StaticClass();
	GameStateClass = AKBGameState::StaticClass();

	ScoreToClear = 2;
}

void AKBGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	KBGameState = Cast<AKBGameState>(GameState);
}

void AKBGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto KBPlayerState = Cast<AKBPlayerState>(NewPlayer->PlayerState);
	EROCHECK(nullptr != KBPlayerState);
	KBPlayerState->InitPlayerDate();
}

void AKBGameMode::AddScore(AKBPlayerController * ScoredPlayer)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto KBPlayerController = Cast<AKBPlayerController>(It->Get());
		if ((nullptr != KBPlayerController) && (ScoredPlayer == KBPlayerController))
		{
			KBPlayerController->AddGameScore();
			break;
		}
	}

	KBGameState->AddGameScore();
	
	if (KBGameState->GetTotalGameScore() >= ScoreToClear)
	{
		KBGameState->SetGameCleared();

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It;++It)
		{
			const auto KBPlayerController = Cast<AKBPlayerController>(It->Get());
			if (nullptr != KBPlayerController)
			{
				KBPlayerController->ShowResultUI();
			}
		}
	}
}
