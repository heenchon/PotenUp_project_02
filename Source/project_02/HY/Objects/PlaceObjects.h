// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlaceObjects.generated.h"

class AUsable_Item;
class ABasePlayerState;

UCLASS()
class PROJECT_02_API APlaceObjects : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APlaceObjects();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ProcessDuration;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ABasePlayerState* PS;
	
private:
	FTimerHandle ProcessTimerHandle;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Place();
	void UnPlace();
	virtual void Interact();
	virtual void Interact(AUsable_Item* input, int curItemIndex);
	void ProcessStart();
	virtual void ProcessComplete();

	bool IsEnabled = false;
	bool CanBuild = true;
	
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> OriginMaterials;
	
	FORCEINLINE void SetDefaultMaterial();
	
	void SetWireframeMaterial(UMaterial* NewMaterial);

private:
	UFUNCTION()
	void OnBeginOverlapMesh(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlapMesh(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};
