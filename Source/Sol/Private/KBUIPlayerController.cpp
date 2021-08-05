// Fill out your copyright notice in the Description page of Project Settings.


#include "KBUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void AKBUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	EROCHECK(nullptr != UIWidgetClass);

	UIWidegetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	EROCHECK(nullptr != UIWidegetInstance);

	UIWidegetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidegetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}

