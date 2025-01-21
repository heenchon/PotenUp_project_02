// Fill out your copyright notice in the Description page of Project Settings.


#include "Usable_Item.h"
#include "project_02/Helper/ItemHelper.h"

// Sets default values
AUsable_Item::AUsable_Item()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToChannel(ECC_EngineTraceChannel1,ECR_Overlap);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
}

void AUsable_Item::UpdateItemInfo(const uint32 RemainCount)
{
	if (RemainCount == 0)
	{
		Destroy();
	} else
	{
		ItemMetaInfo.SetCurrentCount(RemainCount);
	}
}

// Called when the game starts or when spawned
void AUsable_Item::BeginPlay()
{
	Super::BeginPlay();
	const FItemInfoData ItemData = FItemHelper::GetItemInfoById(GetWorld(), ItemId);
	
	ItemMetaInfo.SetId(ItemId);
	// TODO: 해당 하드코딩은 추후 랜덤 값 or 지정 값으로 변경 가능성 있음
	ItemMetaInfo.SetCurrentCount(1);
	ItemMetaInfo.SetMetaData(ItemData.GetMetaData());
}

void AUsable_Item::Use()
{
}

void AUsable_Item::ApplyTo()
{
}

