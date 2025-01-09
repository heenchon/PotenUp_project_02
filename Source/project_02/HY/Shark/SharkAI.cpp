// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAI.h"
#include "SharkStateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "State/SharkIdle.h"              
#include "State/SharkMovingToRaft.h"      
#include "State/SharkAttackingRaft.h"     

// Sets default values
ASharkAI::ASharkAI()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ASharkAI::BeginPlay()
{
	Super::BeginPlay();
	
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	StateMachine = NewObject<USharkStateMachine>();
	
	// StateMachine->Initialize();
	auto Idle = NewObject<USharkIdle>();                  
	auto MoveToRaft = NewObject<USharkMovingToRaft>();    
	auto AttackRaft = NewObject<USharkAttackingRaft>();

	StateMachine->ChangeState(Idle,this);
}

// Called every frame
void ASharkAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StateMachine->Update(this, DeltaTime);
}

