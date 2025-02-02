#pragma once

#include "CoreMinimal.h"
#include "project_02/Tool/InteractiveItem.h"
#include "GameFramework/Actor.h"
#include "project_02/Tool/InteractiveInterface.h"
#include "Sail.generated.h"

UCLASS()
class PROJECT_02_API ASail : public AInteractiveItem, public IInteractiveInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASail();
	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* StaticMesh;
	
	//TODO: 에셋 찾으면 제거
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* Arrow;

	UPROPERTY(EditAnywhere)
	float MaxSailStrength = 6.0f;
	UPROPERTY(EditAnywhere)
	double RotationMultiplier = 2.0;

	virtual FString GetDisplayText() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ARaftGameState* RaftGameState;
	FVector3d WindDirection;
	float WindStrength;
	
	UPROPERTY(EditAnywhere)
	class ARaft* Raft;
	UPROPERTY(EditAnywhere)
	class AController* PlayerController;
	float PlayerYawOrigin;
	float SailYawOrigin;
	
	float MinSailStrength;
	FVector MyDirection;
	bool bSailOn = false;
	bool bIsRotate = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void StartInteractive() override;
	
	void SailToggle();
	void ChangeStrength(float myStrength);
	float CompareDirection(FVector3d myDir, FVector3d windDir);
	void RotateSail();
	void RotateInit(float y);
	void RotateStop();
	void SetRaft(ARaft* raft);
};


