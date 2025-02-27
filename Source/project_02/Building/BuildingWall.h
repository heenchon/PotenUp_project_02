﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingActor.h"
#include "BuildingWall.generated.h"

UCLASS()
class PROJECT_02_API ABuildingWall : public ABuildingActor
{
	GENERATED_BODY()

public:
	ABuildingWall();
	virtual void OnWireframeActive() override;
	virtual void OnWireframeInactive() override;
	virtual void UpdateBuildData(const UPrimitiveComponent* TargetComp, ABuildingActor* ChildBuild) override;
	
	TObjectPtr<UBoxComponent> GetBoxByDirection(const EBlockPos Direction, const bool IsReverse = false) const;
	void UpdateWireframeBoxInfo();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> ForwardFloorBodyBox;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> BackwardFloorBodyBox;
};
