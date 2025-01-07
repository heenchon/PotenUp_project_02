#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class AHookRope;
class USpringArmComponent;
class UCameraComponent;
class USurvivalComponent;
class UInputComponent;
class UInputMappingContext;
class UInputAction;

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
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	UInputAction* LookInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = true))
	UInputAction* JumpInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Data"
		, meta = (AllowPrivateAccess = true))
	FDataTableRowHandle AnimationInfo;

	UFUNCTION()
	void MoveTo(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);
};
