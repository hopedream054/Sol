// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "GameFramework/PlayerController.h"
#include "KBUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API AKBUIPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
	class UUserWidget* UIWidegetInstance;
};
