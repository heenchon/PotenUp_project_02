// Fill out your copyright notice in the Description page of Project Settings.


#include "Usable_Item.h"


// Sets default values
AUsable_Item::AUsable_Item()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AUsable_Item::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUsable_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUsable_Item::Use()
{
}

void AUsable_Item::ApplyTo()
{
}

