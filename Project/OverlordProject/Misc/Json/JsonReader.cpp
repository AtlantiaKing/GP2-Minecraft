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
