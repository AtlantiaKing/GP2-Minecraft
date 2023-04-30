#include "stdafx.h"
#include "JsonReader.h"

std::unordered_map<std::string,Block*> JsonReader::ReadBlocks()
{
	std::ifstream input{ "Resources/Data/blocks.json" };
	std::stringstream jsonStream{};
	jsonStream << input.rdbuf();
	rapidjson::Document d;
	d.Parse(jsonStream.str().c_str());

	std::unordered_map<std::string, Block*> blocks{};

	for (const auto& block : d.GetArray())
	{
		ReadBlock(block, blocks);
	}

	return blocks;
}

void JsonReader::ReadBlock(const rapidjson::Value& block, std::unordered_map<std::string, Block*>& blocks)
{
	Block* pBlock{ new Block{} };

	pBlock->type = static_cast<BlockType>(block["id"].GetInt());

	// Calculate the time it takes to break the current block
	// Reference: https://minecraft.fandom.com/wiki/Breaking#Calculation
	pBlock->breakTime =  1.0f / (1.0f / block["hardness"].GetFloat() / 30.0f) / 20.0f;
	
	const auto& dropIt = block.FindMember("drop");
	pBlock->dropBlock = dropIt != block.MemberEnd() ? blocks[dropIt->value.GetString()] : pBlock;

	blocks[block["name"].GetString()] = pBlock;
}

std::unordered_map<std::string, Biome> JsonReader::ReadBiomes(const std::unordered_map<std::string, Block*>& blocks)
{
	std::ifstream input{ "Resources/Data/biomes.json" };
	std::stringstream jsonStream{};
	jsonStream << input.rdbuf();
	rapidjson::Document d;
	d.Parse(jsonStream.str().c_str());

	std::unordered_map<std::string, Biome> chunks{};

	for (const auto& chunk : d.GetArray())
	{
		ReadBiome(chunk, chunks, blocks);
	}

	return chunks;
}

void JsonReader::ReadBiome(const rapidjson::Value& chunk, std::unordered_map<std::string, Biome>& chunks, const std::unordered_map<std::string, Block*>& blocks)
{
	Biome data{};

	data.topLayer = blocks.at(chunk["top_layer"].GetString());

	const auto& layers{ chunk["layers"] };
	for (const auto& layer : layers.GetArray())
	{
		BlockLayer bLayer{};
		bLayer.pBlock = blocks.at(layer["block"].GetString());

		// If no size is given in a layer, it should take up all the rest of the space
		const auto& sizeIt = layer.FindMember("size");
		bLayer.size = sizeIt != layer.MemberEnd() ? sizeIt->value.GetInt() : -1;

		data.layers.push_back(bLayer);
	}

	const auto& beachData{ chunk["beach"] };

	BlockLayer beach{};
	beach.pBlock = blocks.at(beachData["block"].GetString());
	beach.size = beachData["size"].GetInt();
	data.beach = beach;

	chunks[chunk["name"].GetString()] = data;

	// TODO: read small and big vegitation structures
}
