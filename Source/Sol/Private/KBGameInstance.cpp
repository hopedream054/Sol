// Fill out your copyright notice in the Description page of Project Settings.


#include "KBGameInstance.h"

UKBGameInstance::UKBGameInstance()
{
	//데이터 테이블 적용
	FString CharacterDataPath = TEXT("DataTable'/Game/Player/GameData/ABCharacterData.ABCharacterData'");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ABCHARACTER(*CharacterDataPath);
	EROCHECK(DT_ABCHARACTER.Succeeded());
	ABCharacterTable = DT_ABCHARACTER.Object;
	EROCHECK(ABCharacterTable->GetRowMap().Num() > 0);
	//데이터 테이블 적용 끝

	//https://github.com/smarthug/UE4-CPP-Book 이득우책 깃
}

void UKBGameInstance::Init()
{
	Super::Init();
	//UE_LOG(Sol, Warning, TEXT("GI\n"));

	//데이터 테이블 활용
	UE_LOG(Sol, Warning, TEXT("Drop Exp of Level 20 KBCharacter : %d"),GetKBCharacterData(20)->Attack);
}

//데이터 테이블을 활용해서 원하는 레벨을 기입해서 불러온다.
FABCharacterData * UKBGameInstance::GetKBCharacterData(int32 Level)
{
	return ABCharacterTable->FindRow<FABCharacterData>(*FString::FromInt(Level),TEXT(""));
}
