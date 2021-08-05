// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

//블루프린트에서 사용하기 위해서는 UENUM(BlueprintType)을 선언하고 uint8로 기반유형을 지정해야 한다.
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINT, //캐릭터 생성 전의 스테이트
	LOADING, //선택한 캐릭터 애셋을 로딩하는 스테이트
	READY, //캐릭터 애셋 로딩이 완료된 스테이트
	DEAD //HP를 소진해 사망할 때 발생하는 스테이트
};

DECLARE_LOG_CATEGORY_EXTERN(Sol, Log, All);


#define EROCHECK(Expr, ...) {if(!(Expr)) {UE_LOG(Sol,Error , TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}