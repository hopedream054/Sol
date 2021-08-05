// Fill out your copyright notice in the Description page of Project Settings.


#include "KWeaponGun.h"

// Sets default values
AKWeaponGun::AKWeaponGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(
		TEXT("SkeletalMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/SMG11/SK_SMG11_X.SK_SMG11_X'"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AKWeaponGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKWeaponGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

