// Fill out your copyright notice in the Description page of Project Settings.


#include "KBGameplayWidget.h"
#include"Components/Button.h"
#include"KBPlayerController.h"


void UKBGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	

	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UKBGameplayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UKBGameplayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UKBGameplayWidget::OnRetryGameClicked);
	}
}
void UKBGameplayWidget::OnResumeClicked()
{
	auto KBPlayerController = Cast<AKBPlayerController>(GetOwningPlayer());
	EROCHECK(nullptr!= KBPlayerController);

	RemoveFromParent();
	KBPlayerController->ChangeInputMode(true);
	KBPlayerController->SetPause(false);
}

void UKBGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UKBGameplayWidget::OnRetryGameClicked()
{
	auto KBPlayerController = Cast<AKBPlayerController>(GetOwningPlayer());
	EROCHECK(nullptr != KBPlayerController);
	KBPlayerController->RestartLevel();
}
