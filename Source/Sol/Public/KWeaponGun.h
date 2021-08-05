// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Sol.h"
#include "GameFramework/Actor.h"
#include "KWeaponGun.generated.h"

UCLASS()
class SOL_API AKWeaponGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKWeaponGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

};
