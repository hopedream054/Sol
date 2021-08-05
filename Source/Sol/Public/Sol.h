// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

//�������Ʈ���� ����ϱ� ���ؼ��� UENUM(BlueprintType)�� �����ϰ� uint8�� ��������� �����ؾ� �Ѵ�.
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINT, //ĳ���� ���� ���� ������Ʈ
	LOADING, //������ ĳ���� �ּ��� �ε��ϴ� ������Ʈ
	READY, //ĳ���� �ּ� �ε��� �Ϸ�� ������Ʈ
	DEAD //HP�� ������ ����� �� �߻��ϴ� ������Ʈ
};

DECLARE_LOG_CATEGORY_EXTERN(Sol, Log, All);


#define EROCHECK(Expr, ...) {if(!(Expr)) {UE_LOG(Sol,Error , TEXT("ASSERTION : %s"), TEXT("'"#Expr"'")); return __VA_ARGS__;}}