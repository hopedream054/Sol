// Fill out your copyright notice in the Description page of Project Settings.


#include "KItemBox.h"
#include"KWeaponGun.h"
#include"KBCharacter.h"

// Sets default values
AKItemBox::AKItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(Trigger);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Magazine(
		TEXT("StaticMesh'/Game/FPS_Weapon_Bundle/Weapons/Meshes/KA74U/SM_KA74U_Mag_Empty_X.SM_KA74U_Mag_Empty_X'"));
	if (SM_Magazine.Succeeded())
	{
		Box->SetStaticMesh(SM_Magazine.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Forst(
		TEXT("ParticleSystem'/Game/ParagonRevenant/FX/Particles/Revenant/Skins/FrostKing/P_Revenant_FrostKing_Head.P_Revenant_FrostKing_Head'"));
	if (P_Forst.Succeeded())
	{
		Effect->SetTemplate(P_Forst.Object); // �� ParticleSystemComponent���� ����ϴ� ParticleSystem ����
		Effect->bAutoActivate = false;
	}


	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = AKWeaponGun::StaticClass();


}

// Called when the game starts or when spawned
void AKItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AKItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AKItemBox::OnCharacterOverlap);

}

// Called every frame
void AKItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKItemBox::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(Sol, Warning, TEXT("chracter_overlap"));

	auto KCharacter = Cast<AKBCharacter>(OtherActor);
	EROCHECK(nullptr != KCharacter);

	if (nullptr != KCharacter && nullptr != WeaponItemClass)
	{
		if (KCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<AKWeaponGun>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			KCharacter->SetWeapon(NewWeapon);

			//��ƼŬ��
			Effect->Activate(true);//��ƼŬ Ȱ��ȭ
			Box->SetHiddenInGame(true, true);//���ӳ����� �Ⱥ��̰�
			SetActorEnableCollision(false); //�浹��������
			Effect->OnSystemFinished.AddDynamic(this, &AKItemBox::OnEffectFinished);//��ƼŬ�� ������ �������ִ� �����۹ڽ��� ������ �������ش�.
		}
		else
		{
			UE_LOG(Sol, Warning, TEXT("You have aleady weapon. \n"));
		}
	}
}

void AKItemBox::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	Destroy();
}

