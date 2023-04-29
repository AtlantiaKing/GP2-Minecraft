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
	pBlock->hardness = block["hardness"].GetFloat();

	blocks[block["name"].GetString()] = pBlock;
}
