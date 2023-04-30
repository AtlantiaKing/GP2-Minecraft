#pragma once

enum class FaceDirection : BYTE
{
	FORWARD,
	BACK,
	RIGHT,
	LEFT,
	UP,
	BOTTOM
};

enum class FaceType : BYTE
{
	DIRT,
	GRASS,
	GRASS_SIDE,
	SAND,
	WATER,
	BEDROCK,
	STONE,
	COBBLESTONE,
	SANDSTONE_SIDE,
	SANDSTONE_BOTTOM,
	SANDSTONE_TOP
};

enum class BlockType : BYTE
{
	DIRT,
	GRASS,
	GRASS_SIDE,			// NOT USED, JUST PADDING
	SAND,
	WATER,
	BEDROCK,
	STONE,
	COBBLESTONE,
	SANDSTONE,
	SANDSTONE_BOTTOM,	// NOT USED, JUST PADDING
	SANDSTONE_TOP		// NOT USED, JUST PADDING
};

struct Block
{
	BlockType type{};
	float breakTime{};
	Block* dropBlock{};
};

struct BlockLayer
{
	Block* pBlock{};
	int size{};
};

struct Biome
{
	Block* topLayer{};
	std::vector<BlockLayer> layers{};
	BlockLayer beach{};
	// TODO: Add vegitation structures
};