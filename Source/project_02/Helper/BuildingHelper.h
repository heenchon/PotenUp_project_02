#pragma once


enum class EBlockCategory;
enum class EBlockType;
class ABuildingActor;

class FBuildingHelper
{
public:
	static TSubclassOf<ABuildingActor> GetBuildingClass(
			const UWorld* World, const EBlockType BlockType
			, const EBlockCategory BlockCategory);
};
