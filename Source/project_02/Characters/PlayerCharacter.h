#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UPlayerEquipmentUI;
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

	FORCEINLINE FDataTableRowHandle GetAnimationData() const { return AnimationInfo; }
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Survival)
	TObjectPtr<USurvivalComponent> SurvivalComponent;
	
protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// IA와 함수 1:1 매핑하게 하는 것도 방법으로 보임
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> MoveInputAction;

	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> LookInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> JumpInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractiveInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InventoryAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Data"
		, meta = (AllowPrivateAccess = true))
	FDataTableRowHandle AnimationInfo;

	// TODO: 추후 HookRope 자체를 공통화해서 상호작용하는 액터 자체를 적용할 예정
	// 우선은 하드코딩으로 처리
	UPROPERTY()
	TObjectPtr<AHookRope> TestInteractiveItem;
	
	UPROPERTY(EditDefaultsOnly, Category = "Data"
		, meta = (AllowPrivateAccess = true))
	TSubclassOf<AHookRope> TestInteractiveItemClass;

	UFUNCTION()
	void MoveTo(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);

	UFUNCTION()
	void OnInteractiveHolding();
	
	UFUNCTION()
	void OnInteractiveEnd();

	// TODO: UI 관련은 공통 컴포넌트로 이전해도 무방해보임
	UPROPERTY(EditDefaultsOnly, Category="Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPlayerEquipmentUI> EquipmentUIClass;
	
	TObjectPtr<UPlayerEquipmentUI> EquipmentUI;

	bool IsOpenInventory = false;
	
	UFUNCTION()
	void ToggleInventory();
};
