#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveHook.generated.h"

class UBoxComponent;
class ATrash;
class USphereComponent;

UENUM()
enum class EHookStatus: uint8
{
	Idle,
	Launched,
	Fixed,
	Pulled,
};

UCLASS()
class PROJECT_02_API AInteractiveHook : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveHook();

	FORCEINLINE EHookStatus GetHookStatus() const { return HookStatus; }
	FORCEINLINE void SetHookStatus(const EHookStatus NewState) { HookStatus = NewState; }

	FORCEINLINE UStaticMeshComponent* GetHookMesh() const { return HookMesh; };

	void StartLaunch(const FVector& MoveToVector, const uint8 NewPower);

	bool PullHook(const FVector& TargetLocation);

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float GravityScale;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float PowerPercent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float HookPullSpeed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float HookCalcRadius;
	
	EHookStatus HookStatus = EHookStatus::Idle;
	
	FVector MoveToPos;
	
	uint8 Power;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> DefaultRoot;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> HookMesh;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> BoxCollision;
	
	UFUNCTION()
	void OnOverlapHookGrab(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult &SweepResult
	);

	double GetDistanceBetweenMoveToAndCurrentLocation(); 

	TArray<TObjectPtr<ATrash>> AttachTrashList;
};
