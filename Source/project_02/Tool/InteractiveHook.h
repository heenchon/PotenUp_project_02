#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveHook.generated.h"

class UBuoyancyComponent;
class USphereComponent;

UCLASS()
class PROJECT_02_API AInteractiveHook : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveHook();

	FORCEINLINE UStaticMeshComponent* GetHookMesh() const { return HookMesh; };
protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USphereComponent> GrabOverlapBox;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> HookMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "Buoyancy", meta = (AllowPrivateAccess = true))
	TObjectPtr<UBuoyancyComponent> Buoyancy;
	
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
