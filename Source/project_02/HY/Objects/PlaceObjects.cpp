// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceObjects.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/Building/BuildingFloor.h"
#include "project_02/HY/RaftGameState.h"
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
	OriginMaterials = StaticMesh->GetMaterials();
	
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlapMesh);
	StaticMesh->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnEndOverlapMesh);
}

// Called every frame
void APlaceObjects::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlaceObjects::Place()
{
	IsEnabled = true;
}

void APlaceObjects::UnPlace()
{
}

void APlaceObjects::Interact()
{
	if (!IsEnabled)
	{
		return;
	}
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

void APlaceObjects::SetDefaultMaterial()
{
	for (int i = 0; i < OriginMaterials.Num(); i++)
	{
		StaticMesh->SetMaterial(i, OriginMaterials[i]);
	}
}

void APlaceObjects::SetWireframeMaterial(UMaterial* NewMaterial)
{
	for (int i = 0; i < StaticMesh->GetMaterials().Num(); i++)
	{
		StaticMesh->SetMaterial(i, NewMaterial);
	}
}

void APlaceObjects::OnBeginOverlapMesh(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ABuildingFloor::StaticClass()))
	{
		CanBuild = false;
	}
}

void APlaceObjects::OnEndOverlapMesh(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!OtherActor->IsA(ABuildingFloor::StaticClass()))
	{
		CanBuild = true;
	}
}

void APlaceObjects::UpdateBuildData(const ABuildingActor* ParentActor)
{
	if (ARaftGameState* GS = GetWorld()->GetGameState<ARaftGameState>())
	{
		GS->GetRaftBuildMetaData();
	}
}
