#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveHook.generated.h"

class USphereComponent;

UENUM()
enum class EHookStatus: uint8
{
	Idle,
	Launched,
};

UCLASS()
class PROJECT_02_API AInteractiveHook : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveHook();

	FORCEINLINE UStaticMeshComponent* GetHookMesh() const { return HookMesh; };

	void StartLaunch(const FVector& MoveToVector, const uint8 NewPower);

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float GravityScale;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float PowerPercent;
	
	EHookStatus HookStatus = EHookStatus::Idle;
	
	FVector MovetoPos;
	uint8 Power;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> DefaultRoot;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> GrabOverlapBox;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> HookMesh;
	
	UFUNCTION()
	void OnOverlapHookGrab(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult &SweepResult
	);
};
