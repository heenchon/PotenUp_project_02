#pragma once

struct FItemInfoData;

class FItemHelper
{
public:
	static FItemInfoData GetItemInfoById(const UWorld* World, const uint16 Id);
};
