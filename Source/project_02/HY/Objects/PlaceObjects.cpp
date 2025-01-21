// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceObjects.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/Player/BasePlayerState.h"

// Sets default values
APlaceObjects::APlaceObjects()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	SetRootComponent(Root);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void APlaceObjects::BeginPlay()
{
	Super::BeginPlay();
	PS = UGameplayStatics::GetPlayerPawn(GetWorld(),0)->GetPlayerState<ABasePlayerState>();
}

// Called every frame
void APlaceObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlaceObjects::Place()
{
}

void APlaceObjects::UnPlace()
{
}

void APlaceObjects::Interact(AUsable_Item* input, int curItemIndex)
{
}

void APlaceObjects::ProcessStart()
{
	GetWorld()->GetTimerManager().SetTimer(ProcessTimerHandle,this,&APlaceObjects::ProcessComplete,ProcessDuration,false);
}

//TODO: 사운드, 효과 
void APlaceObjects::ProcessComplete()
{
}

