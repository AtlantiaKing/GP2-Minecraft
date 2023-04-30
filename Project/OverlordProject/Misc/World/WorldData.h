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
	COBBLESTONE
};

enum class BlockType : BYTE
{
	DIRT,
	GRASS,
	GRASS_SIDE, // NOT USED, JUST PADDING
	SAND,
	WATER,
	BEDROCK,
	STONE,
	COBBLESTONE
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