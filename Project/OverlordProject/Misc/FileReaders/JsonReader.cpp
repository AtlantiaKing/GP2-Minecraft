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

std::vector<std::string> JsonReader::ReadBlockTypes()
{
	std::vector<std::string> blockTypes{};

	std::ifstream input{ "Resources/Data/blocktypes.json" };
	std::stringstream jsonStream{};
	jsonStream << input.rdbuf();
	rapidjson::Document d;
	d.Parse(jsonStream.str().c_str());

	for (const auto& blockType : d.GetArray())
	{
		blockTypes.emplace_back(blockType.GetString());
	}

	return blockTypes;
}

void JsonReader::ReadBlock(const rapidjson::Value& block, std::unordered_map<std::string, Block*>& blocks)
{
	Block* pBlock{ new Block{} };

	pBlock->type = static_cast<BlockType>(block["id"].GetInt());

	// Calculate the time it takes to break the current block
	// Reference: https://minecraft.fandom.com/wiki/Breaking#Calculation
	pBlock->breakTime =  1.0f / (1.0f / block["hardness"].GetFloat() / 30.0f) / 20.0f;
	
	const auto& dropIt = block.FindMember("drop");
	if (dropIt != block.MemberEnd())
	{
		auto str{ dropIt->value.GetString() };
		if (strcmp(str, "none") == 0) pBlock->dropBlock = nullptr;
		else pBlock->dropBlock = blocks[str];
	}
	else
	{
		pBlock->dropBlock = pBlock;
	}

	const auto& transparentIt = block.FindMember("transparent");
	pBlock->transparent = transparentIt != block.MemberEnd() ? transparentIt->value.GetBool() : false;

	pBlock->mesh = static_cast<BlockMesh>(block["mesh"].GetInt());

	blocks[block["name"].GetString()] = pBlock;
}

std::unordered_map<std::string, Biome> JsonReader::ReadBiomes(const std::unordered_map<std::string, Block*>& blocks, const std::unordered_map<std::string, Structure>& structures)
{
	std::ifstream input{ "Resources/Data/biomes.json" };
	std::stringstream jsonStream{};
	jsonStream << input.rdbuf();
	rapidjson::Document d;
	d.Parse(jsonStream.str().c_str());

	std::unordered_map<std::string, Biome> chunks{};

	for (const auto& chunk : d.GetArray())
	{
		ReadBiome(chunk, chunks, blocks, structures);
	}

	return chunks;
}

void JsonReader::ReadBiome(const rapidjson::Value& chunk, std::unordered_map<std::string, Biome>& biomes, const std::unordered_map<std::string, Block*>& blocks, const std::unordered_map<std::string, Structure>& structures)
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

		data.layers.emplace_back(bLayer);
	}

	const auto& beachData{ chunk["beach"] };

	BlockLayer beach{};
	beach.pBlock = blocks.at(beachData["block"].GetString());
	beach.size = beachData["size"].GetInt();
	data.beach = beach;

	const auto structureIt{ structures.find(chunk["big_vegitation"].GetString()) };
	data.bigVegitation = structureIt != structures.end() ? &structures.at(chunk["big_vegitation"].GetString()) : nullptr;

	const auto smallStructureIt{ structures.find(chunk["small_vegitation"].GetString()) };
	data.smallVegitation = structureIt != structures.end() ? &structures.at(chunk["small_vegitation"].GetString()) : nullptr;

	biomes[chunk["name"].GetString()] = data;
}

std::unordered_map<std::string, Structure> JsonReader::ReadStructures(const std::unordered_map<std::string, Block*>& blocks)
{
	std::ifstream input{ "Resources/Data/structures.json" };
	std::stringstream jsonStream{};
	jsonStream << input.rdbuf();
	rapidjson::Document d;
	d.Parse(jsonStream.str().c_str());

	std::unordered_map<std::string, Structure> structures{};

	for (const auto& structureName : d.GetArray())
	{
		std::stringstream structureFilePath{};
		structureFilePath << "Resources/Data/" << structureName.GetString() << ".json";

		std::ifstream structureFile{ structureFilePath.str() };
		std::stringstream structureStream{};
		structureStream << structureFile.rdbuf();
		rapidjson::Document structure;
		structure.Parse(structureStream.str().c_str());

		ReadStructure(structure, structureName.GetString(), structures, blocks);
	}

	return structures;
}

void JsonReader::ReadStructure(const rapidjson::Document& structure, const std::string& name, std::unordered_map<std::string, Structure>& structures, const std::unordered_map<std::string, Block*>& blocks)
{
	Structure s{};

	const auto& blocksArray{ structure["blocks"] };

	for (const auto& blockData : blocksArray.GetArray())
	{
		StructureBlock b{};

		b.pBlock = blocks.at(blockData["block"].GetString());

		const auto& position{ blockData["position"] };
		const XMINT3 pos
		{
			position["x"].GetInt(),
			position["y"].GetInt(),
			position["z"].GetInt()
		};

		b.position = pos;

		s.blocks.emplace_back(b);
	}

	s.pSpawnOnBlock = blocks.at(structure["spawn_block"].GetString());

	structures[name] = s;
}
