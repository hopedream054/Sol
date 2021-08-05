// Fill out your copyright notice in the Description page of Project Settings.


#include "KBPlayerController.h"
#include "KBHUDWidget.h"
#include "KBPlayerState.h"
#include"KBCharacter.h"
#include"KBGameplayWidget.h"
#include"KBGamePlayResultWidget.h"
#include"KBGameState.h"

AKBPlayerController::AKBPlayerController()
{
	static ConstructorHelpers::FClassFinder<UKBHUDWidget> UI_HUD_C(TEXT(
		"WidgetBlueprint'/Game/UI/UI_HUD.UI_HUD_C'"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UKBGameplayWidget> UI_MENU_C(TEXT(
		"WidgetBlueprint'/Game/UI/UI_Menu.UI_Menu_C'"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UKBGameplayWidget> UI_RESULT_C(TEXT(
		"WidgetBlueprint'/Game/UI/UI_Result.UI_Result_C'"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}

void AKBPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(Sol, Warning, TEXT("AI PostInitializeComponents"));
}


void AKBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	

	//FInputModeGameOnly InputMode;
	//SetInputMode(InputMode);

	ChangeInputMode(true);

	HUDWidget = CreateWidget<UKBHUDWidget>(this, HUDWidgetClass);
	EROCHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);

	KBPlayerState = Cast<AKBPlayerState>(PlayerState);
	EROCHECK(nullptr != KBPlayerState);
	HUDWidget->BindPlayerState(KBPlayerState);
	KBPlayerState->OnPlayerStateChanged.Broadcast();

	ResultWidget = CreateWidget<UKBGameplayResultWidget>(this, ResultWidgetClass);
	EROCHECK(nullptr!= ResultWidget);

}

void AKBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this,
		&AKBPlayerController::OnGamePause);
}

void AKBPlayerController::OnGamePause()
{
	MenuWidget = CreateWidget<UKBGameplayWidget>(this, MenuWidgetClass);
	EROCHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);

	SetPause(true);
	ChangeInputMode(false);
}

UKBHUDWidget * AKBPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AKBPlayerController::NPCKill(AKBCharacter * KilledNPC) const
{
	KBPlayerState->AddExp(KilledNPC->GetExp());
}

void AKBPlayerController::AddGameScore() const
{
	KBPlayerState->AddGameScore();
}

void AKBPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void AKBPlayerController::ShowResultUI()
{
	auto KBGameState = Cast<AKBGameState>(UGameplayStatics::GetGameState(this));
	EROCHECK(nullptr != KBGameState);
	ResultWidget->BindGameState(KBGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}
