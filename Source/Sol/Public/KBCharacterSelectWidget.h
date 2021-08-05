// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "Blueprint/UserWidget.h"
#include "KBCharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class SOL_API UKBCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	void NextCharacter(bool bForward = true);

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnPrevClicked();

	UFUNCTION()
	void OnNextClicked();

	UFUNCTION()
	void OnConfirmClicked();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 CurrentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	int32 MaxIndex;

	UPROPERTY()
		class UButton* PrevButton;

	UPROPERTY()
		class UButton* NextButton;

	UPROPERTY()
		class UEditableTextBox* TextBox;

	UPROPERTY()
		class UButton* ConfirmButton;

	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;
};
