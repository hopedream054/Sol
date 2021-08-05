// Fill out your copyright notice in the Description page of Project Settings.


#include "KBCharacterWidget.h"
#include "KBChracterStateComponent.h"
#include "Components/ProgressBar.h"

void UKBCharacterWidget::BindCharacterStat(class UKBChracterStateComponent* NewCharacterStat)
{
	EROCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UKBCharacterWidget::UpdateHPWidget);

	UE_LOG(Sol, Warning, TEXT("BindCharacterStat"));
	NewCharacterStat->OnHPChanged.AddLambda([this]()->void {
		
		if (CurrentCharacterStat.IsValid())
		{
			UE_LOG(Sol, Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
		}
		
		
	});
}

void UKBCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	EROCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
	
}

void UKBCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
			HPProgressBar->RenderOpacity = 0;
		}
		//RenderOpacity ����� ����
		UE_LOG(Sol, Warning, TEXT("UpdateHPHPHP %s : %f"),*GetFName().ToString(), CurrentCharacterStat->GetHPRatio());
		
	}
}
