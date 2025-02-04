// Fill out your copyright notice in the Description page of Project Settings.


#include "Sail.h"
#include "../RaftGameState.h"
#include "../Raft/Raft.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/PlayerCharacter.h"

ASail::ASail()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(Root);
	ConstructorHelpers::FObjectFinder<UStaticMesh>DefaultMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (DefaultMesh.Succeeded()) StaticMesh->SetStaticMesh(DefaultMesh.Object);
	
	// StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	// StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	// StaticMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Overlap);

	Arrow = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Root);
	Arrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> arrow(TEXT("/Script/Engine.StaticMesh'/Engine/EditorResources/FieldNodes/_Resources/SM_FieldArrow.SM_FieldArrow'"));
	if (arrow.Succeeded()) Arrow->SetStaticMesh(arrow.Object);
	
	MinSailStrength = 1.0f / MaxSailStrength *0.5f;
}

void ASail::BeginPlay()
{
	Super::BeginPlay();
	// UE_LOG(LogTemp,Warning,TEXT("돛의 Min: %f"), MinSailStrength);
	
	RaftGameState = GetWorld()->GetGameState<ARaftGameState>();
	if (RaftGameState)
	{
		WindDirection = RaftGameState->WindDirection;
		WindStrength = RaftGameState->WindStrength;
	}
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//TODO: 후에 에셋 바꾸면 필요없어질 내용.
	SetActorScale3D(FVector(0.2, 2.0, 1.0));
}

FString ASail::GetDisplayText() const
{
	if (!bSailOn)
	{
		return TEXT("Unfurl sails");
	}
	
	return TEXT("Furl sails");
}


// Called every frame
void ASail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bSailOn)
	{
		MyDirection = this->GetActorRotation().Vector();
		ChangeStrength(CompareDirection(MyDirection, WindDirection));
		RotateSail();
	}
	// UE_LOG(LogTemp, Warning, TEXT("포워드 벡터: %s"), *MyDirection.ToString());
}

 void ASail::SailToggle()
 {
 	if (bSailOn)
 	{
 		bSailOn = false;
 		//TODO: 임시로 애니메이션 대신 스케일 조정
 		SetActorScale3D(FVector(0.2, 2.0, 1.0));
 		Raft->SailStrength = 1.0f;
 	}
 	else
 	{
 		bSailOn = true;
 		SetActorScale3D(FVector(0.2, 2.0, 3.0));
 	}
 	// UE_LOG(LogTemp,Warning,TEXT("bSailOn %d"),bSailOn);
 }

void ASail::ChangeStrength(float myStrength)
{
	Raft->SailStrength = myStrength;
}

float ASail::CompareDirection(FVector3d myDir, FVector3d windDir)
{
	float ForceMultiplier = FMath::Clamp(FVector::DotProduct(myDir, windDir), MinSailStrength, 1.0f);
	// UE_LOG(LogTemp, Warning, TEXT("바람 방향: %s  가중값: %f"), *windDir.ToString(), ForceMultiplier);
	return (MaxSailStrength * ForceMultiplier);
}

void ASail::RotateSail()
{
	if (bIsRotate)
	{
		float yawDiff =  FMath::UnwindDegrees(PlayerController->GetControlRotation().Yaw-PlayerYawOrigin);
		SetActorRotation(FRotator(0,SailYawOrigin+yawDiff*RotationMultiplier,0));
	}
}

void ASail::RotateInit(float y)
{
	// UE_LOG(LogTemp, Warning, TEXT("%f"),yawValue);
	PlayerYawOrigin = y;
	SailYawOrigin = GetActorRotation().Yaw;
	bIsRotate = true;
}

void ASail::RotateStop()
{
	bIsRotate = false;
}

void ASail::SetRaft(ARaft* raft)
{
	Raft = raft;
}

void ASail::StartInteractive()
{
	Super::StartInteractive();
	if (bSailOn)
	{
		bSailOn = false;
		//TODO: 임시로 애니메이션 대신 스케일 조정
		SetActorScale3D(FVector(0.2, 2.0, 1.0));
		Raft->SailStrength = 1.0f;
	}
	else
	{
		bSailOn = true;
		SetActorScale3D(FVector(0.2, 2.0, 3.0));
	}
	// UE_LOG(LogTemp,Warning,TEXT("bSailOn %d"),bSailOn);
}
