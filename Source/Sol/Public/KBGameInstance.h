// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include"Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include"Engine/StreamableManager.h"
#include "KBGameInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FABCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	FABCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

UCLASS()
class SOL_API UKBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UKBGameInstance();

	virtual void Init() override;
	FABCharacterData* GetKBCharacterData(int32 Level);

	
	FStreamableManager StreambleManager;
private:
	UPROPERTY()
	class UDataTable* ABCharacterTable;
};
