// Fill out your copyright notice in the Description page of Project Settings.


#include "KBCharacterSelectWidget.h"
#include"KBCharacterSetting.h"
#include"KBGameInstance.h"
#include"EngineUtils.h"
#include"Animation/SkeletalMeshActor.h"
#include"Components/Button.h"
#include"Components/EditableTextBox.h"
#include"KBSaveGame.h"
#include"KBPlayerState.h"


void UKBCharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	if (CurrentIndex == -1)
	{
		CurrentIndex = MaxIndex - 1;
		
	}
	if (CurrentIndex == MaxIndex)
	{
		CurrentIndex = 0;
	}

	auto CharacterSetting = GetDefault<UKBCharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];

	auto KBGameInstance = GetWorld()->GetGameInstance<UKBGameInstance>();
	EROCHECK(nullptr!= KBGameInstance);
	EROCHECK(TargetComponent.IsValid());

	USkeletalMesh* Asset = KBGameInstance->StreambleManager.LoadSynchronous<USkeletalMesh>(AssetRef);
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}
}
void UKBCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UKBCharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	EROCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	EROCHECK(nullptr != NextButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	EROCHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	EROCHECK(nullptr != ConfirmButton);

	PrevButton->OnClicked.AddDynamic(this, &UKBCharacterSelectWidget::OnPrevClicked);

	NextButton->OnClicked.AddDynamic(this, &UKBCharacterSelectWidget::OnNextClicked);

	ConfirmButton->OnClicked.AddDynamic(this, &UKBCharacterSelectWidget::OnConfirmClicked);
}

void UKBCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UKBCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UKBCharacterSelectWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10)
	{
		return;
	}

	UKBSaveGame* NewPlayerData = NewObject<UKBSaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	auto KBPlayerState = GetDefault<AKBPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, KBPlayerState->SaveSlotName, 0))
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("test01"));
	}
	else
	{
		UE_LOG(Sol, Error, TEXT("SaveGame Error!!"));
	}
}

//592