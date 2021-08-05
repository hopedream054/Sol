// Fill out your copyright notice in the Description page of Project Settings.


#include "KBGameInstance.h"

UKBGameInstance::UKBGameInstance()
{
	//������ ���̺� ����
	FString CharacterDataPath = TEXT("DataTable'/Game/Player/GameData/ABCharacterData.ABCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);
	EROCHECK(DT_ABCHARACTER.Succeeded());
	ABCharacterTable = DT_ABCHARACTER.Object;
	EROCHECK(ABCharacterTable->GetRowMap().Num() > 0);
	//������ ���̺� ���� ��

	//https://github.com/smarthug/UE4-CPP-Book �̵��å ��
}

void UKBGameInstance::Init()
{
	Super::Init();
	//UE_LOG(Sol, Warning, TEXT("GI\n"));

	//������ ���̺� Ȱ��
	UE_LOG(Sol, Warning, TEXT("Drop Exp of Level 20 KBCharacter : %d"),GetKBCharacterData(20)->Attack);
}

//������ ���̺��� Ȱ���ؼ� ���ϴ� ������ �����ؼ� �ҷ��´�.
FABCharacterData * UKBGameInstance::GetKBCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level),TEXT(""));
}
