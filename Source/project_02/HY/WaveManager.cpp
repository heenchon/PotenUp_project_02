#include "WaveManager.h"
#include "GerstnerWaterWaves.h"
#include "WaterBodyOceanActor.h"
#include "GerstnerWaterWaveSubsystem.h"

AWaveManager::AWaveManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWaveManager::BeginPlay()
{
	Super::BeginPlay();
	ChangeWaves();
}

// Called every frame
void AWaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaveManager::ChangeWaves()
{
	TArray<FGerstnerWave> OutWaves;
	GenerateGerstnerWaves(OutWaves);



	// auto* newWaterWave = NewObject<UWaterWavesBase>(this, GWaterWaves, TEXT("GWaterWaves"));
	// BodyOcean->SetWaterWaves(newWaterWave);
	// GWaterWaves->RecomputeWaves(true);
	// TArray<FGerstnerWave> OutWaves;
	// OutWaves.Add({WaveLength, Amplitude,Steepness, Direction,
	// 	WaveVector,WaveSpeed,0.0f,0.0f,0.0f});
	// GWaterWaves->GerstnerWaveGenerator->GenerateGerstnerWaves_Implementation(OutWaves);
	// UE_LOG(LogTemp, Display, TEXT("테스트 %f"), WaveLength);
	// BodyOcean->SetWaterWaves(GWaterWaves);
}

void AWaveManager::GenerateGerstnerWaves_Implementation(TArray<FGerstnerWave>&OutWaves) const
{
	
}