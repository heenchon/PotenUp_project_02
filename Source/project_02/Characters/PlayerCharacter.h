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

protected:
	virtual void BeginPlay() override;

	// virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Equipment, meta = (AllowPrivateAccess = true))
	TSubclassOf<AHookRope> UseItem;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> SpringArm;
	
	UPROPERTY(EditDefaultsOnly, Category = Survival, meta = (AllowPrivateAccess = true))
	TObjectPtr<USurvivalComponent> SurvivalComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input
		, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	// IA와 함수 1:1 매핑하게 하는 것도 방법으로 보임
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookInputAction;
	
	UPROPERTY(EditAnywhere, Category = "Input"
		, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpInputAction;

	UFUNCTION()
	void MoveTo(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);
};
