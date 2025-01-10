#pragma once

UENUM(BlueprintType)
enum class ESharkState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	MoveToTarget UMETA(DisplayName = "MoveToTarget"),
	AttackPlayer UMETA(DisplayName = "AttackPlayer"),
	AttackRaft UMETA(DisplayName = "AttackRaft"),
	RunAway UMETA(DisplayName = "RunAway")
};
