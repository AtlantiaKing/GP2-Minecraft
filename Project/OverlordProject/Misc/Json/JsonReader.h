#pragma once

#include "Misc/World/WorldData.h"

#include <rapidjson/document.h>
#include <unordered_map>

class JsonReader final
{
public:
	std::unordered_map<std::string, Block*> ReadBlocks();
	std::unordered_map<std::string, Biome> ReadBiomes(const std::unordered_map<std::string, Block*>& blocks);
private:
	void ReadBlock(const rapidjson::Value& block, std::unordered_map<std::string, Block*>& blocks);
	void ReadBiome(const rapidjson::Value& chunk, std::unordered_map<std::string, Biome>& chunks, const std::unordered_map<std::string, Block*>& blocks);
};

