
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KBCharacterSetting.generated.h"

UCLASS(config = Sol )
class ARENABATTLESETTING_API UKBCharacterSetting : public UObject
{
	GENERATED_BODY()
public:
	UKBCharacterSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};
