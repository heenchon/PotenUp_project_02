// Fill out your copyright notice in the Description page of Project Settings.


#include "Sail.h"
#include "../RaftGameState.h"
#include "../Raft/Raft.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/PlayerCharacter.h"

ASail::ASail()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	SailMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SailMesh->SetupAttachment(Root);
	ConstructorHelpers::FObjectFinder<UStaticMesh>DefaultMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (DefaultMesh.Succeeded()) SailMesh->SetStaticMesh(DefaultMesh.Object);
	
	SailMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SailMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	SailMesh->SetCollisionResponseToChannel(ECC_GameTraceChannel1,ECR_Overlap);
	
	MinSailStrength = 1.0f / MaxSailStrength;
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
	
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	// UE_LOG(LogTemp,Warning,TEXT("%s"),*player->GetName());
	PlayerController = player->GetController();
	SailToggle();
}

// Called every frame
void ASail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyDirection = this->GetActorRotation().Vector();
	if (bSailOn)
	{
		ChangeStrength(CompareDirection(MyDirection, WindDirection));
	}
	// UE_LOG(LogTemp, Warning, TEXT("포워드 벡터: %s"), *MyDirection.ToString());
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

void ASail::SailToggle()
{
	if (bSailOn)
	{
		bSailOn = false;
		//TODO: 임시로 애니메이션 대신 스케일 조정
		SetActorScale3D(FVector(0.5, 2.5, 1.0));
		Raft->SailStrength = 1.0f;
	}
	else
	{
		bSailOn = true;
		SetActorScale3D(FVector(0.5, 2.5, 3.0));
	}
	// UE_LOG(LogTemp,Warning,TEXT("bSailOn %d"),bSailOn);
}

void ASail::RotateSail()
{
	if (bSailOn)
	{
		// UE_LOG(LogTemp, Warning, TEXT("%f"),(PlayerCharacter->GetController()->GetControlRotation().Yaw));
		UE_LOG(LogTemp, Warning, TEXT("%f"),(PlayerController->GetControlRotation().Yaw));

		FRotator curRot = GetActorRotation();
		// double newYaw= curRot.Yaw + PlayerController->GetControlRotation().Yaw*0.01f;
		// SetActorRotation(FRotator(curRot.Pitch, newYaw, curRot.Roll));
	}
}

void ASail::SetRaft(ARaft* raft)
{
	Raft = raft;
}
