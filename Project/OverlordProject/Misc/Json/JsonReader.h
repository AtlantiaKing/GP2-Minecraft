#pragma once

#include "Misc/World/WorldData.h"

#include <rapidjson/document.h>
#include <unordered_map>

class JsonReader final
{
public:
	std::unordered_map<std::string, Block*> ReadBlocks();
private:
	void ReadBlock(const rapidjson::Value& block, std::unordered_map<std::string, Block*>& blocks);
};

