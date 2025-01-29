#pragma once

UENUM(BlueprintType)
enum class ESharkState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	MoveToRaft UMETA(DisplayName = "MoveToRaft"),
	MoveToPlayer UMETA(DisplayName = "MoveToPlayer"),
	AttackPlayer UMETA(DisplayName = "AttackPlayer"),
	AttackRaft UMETA(DisplayName = "AttackRaft"),
	RunAway UMETA(DisplayName = "RunAway"),
	Turning UMETA(DisplayName = "Turning"),
};
