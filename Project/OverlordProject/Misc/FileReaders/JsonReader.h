#pragma once

#include "Misc/World/WorldData.h"

#include <rapidjson/document.h>
#include <unordered_map>

class JsonReader final
{
public:
	std::unordered_map<std::string, Block*> ReadBlocks();
	std::vector<std::string> ReadBlockTypes();
	std::unordered_map<std::string, Biome> ReadBiomes(const std::unordered_map<std::string, Block*>& blocks, const std::unordered_map<std::string, Structure>& structures);
	std::unordered_map<std::string, Structure> ReadStructures(const std::unordered_map<std::string, Block*>& blocks);
private:
	void ReadBlock(const rapidjson::Value& block, std::unordered_map<std::string, Block*>& blocks);
	void ReadBiome(const rapidjson::Value& chunk, std::unordered_map<std::string, Biome>& biomes, const std::unordered_map<std::string, Block*>& blocks, const std::unordered_map<std::string, Structure>& structures);
	void ReadStructure(const rapidjson::Document& structure, const std::string& name, std::unordered_map<std::string, Structure>& structures, const std::unordered_map<std::string, Block*>& blocks);
};

