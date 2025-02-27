﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UBuildingComponent;
class ASail;
struct FItemInfoData;
class UEntityAnimationInfo;
class UBoxComponent;
class USwimmingComponent;
class UInventoryComponent;
class AHookRope;
class USpringArmComponent;
class UCameraComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;
class USurvivalComponent;

struct FInputActionValue;

UCLASS()
class PROJECT_02_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	FORCEINLINE TObjectPtr<UEntityAnimationInfo> GetAnimationData() const { return AnimationInfo; }
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> SpringArm;

	FORCEINLINE TObjectPtr<USurvivalComponent> GetSurvivalComponent() const { return SurvivalComponent; }
	FORCEINLINE TObjectPtr<UInventoryComponent> GetInventoryComponent() const { return InventoryComponent; }
	FORCEINLINE TObjectPtr<USwimmingComponent> GetSwimmingComponent() const { return SwimmingComponent; }
	FORCEINLINE TObjectPtr<UBuildingComponent> GetBuildingComponent() const { return BuildingComponent; }
	FORCEINLINE TObjectPtr<UBoxComponent> GetChestBox() const { return ChestBox; }

	FORCEINLINE TObjectPtr<AActor> GetMainHandTool() { return MainHandTool; }
	void ClearViewItemOnHand();
	void SetViewItemOnHand(const FItemInfoData& NewItemInfo);

	UPROPERTY()
	ASail* Sail;
	
protected:
	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> ChestBox;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<USurvivalComponent> SurvivalComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<USwimmingComponent> SwimmingComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TObjectPtr<UBuildingComponent> BuildingComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// IA와 함수 1:1 매핑하게 하는 것도 방법으로 보임
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> JumpInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractiveInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> UseInputAction;

	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> RotateInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> SaveInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Data"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UEntityAnimationInfo> AnimationInfo;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Use"
		, meta = (AllowPrivateAccess = true))
	float UseInteractiveRange;

	UPROPERTY()
	TObjectPtr<AActor> FindDroppedActor;

	bool IsInteracting;

	UPROPERTY()
	TObjectPtr<AActor> MainHandTool;

	UFUNCTION()
	void MoveTo(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void GoToUp(const FInputActionValue& Value);
	
	UFUNCTION()
	void OnInteractiveEnd();

	UFUNCTION()
	void UseItem();

	UFUNCTION()
	void OnInteractivePressed();

	//희연
	UFUNCTION()
	void RotatePressed();
	UFUNCTION()
	void RotateReleased();

	UFUNCTION()
	void SaveGame();
	
	void FindToUse();
	
	bool IsBlockAction() const;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Camera", meta = (AllowPrivateAccess = true))
	TSubclassOf<UCameraShakeBase> DamagedCameraShake;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Sound", meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundWave> DamagedSound;
};
