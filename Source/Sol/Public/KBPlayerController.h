// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "GameFramework/PlayerController.h"
#include "KBPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API AKBPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AKBPlayerController();

	virtual void PostInitializeComponents() override;

	class UKBHUDWidget* GetHUDWidget() const;
	void NPCKill(class AKBCharacter* KilledNPC) const;
	void AddGameScore() const;

	void ChangeInputMode(bool bGameMode = true);

	void ShowResultUI();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UKBHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UKBGameplayWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UKBGameplayResultWidget> ResultWidgetClass;
private:
	void OnGamePause();

	UPROPERTY()
	class UKBHUDWidget* HUDWidget;

	UPROPERTY()
	class AKBPlayerState* KBPlayerState;

	UPROPERTY()
	class UKBGameplayWidget* MenuWidget;

	UPROPERTY()
	class UKBGameplayResultWidget* ResultWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};