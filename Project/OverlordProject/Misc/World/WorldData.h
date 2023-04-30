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
	SANDSTONE_TOP,
	OAK_LOG_SIDE,
	OAK_LOG_TOP,
	OAK_LEAVES
};

enum class BlockType : BYTE
{
	DIRT = 0,
	GRASS = 1,
	SAND = 3,
	WATER = 4,
	BEDROCK = 5,
	STONE = 6,
	COBBLESTONE = 7,
	SANDSTONE = 8,
	OAK_LOG = 11,
	OAK_LEAVES = 13
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

struct StructureBlock
{
	Block* pBlock{};
	XMINT3 position{};
};

struct Structure
{
	std::vector<StructureBlock> blocks{};
	Block* pSpawnOnBlock{};
};

struct Biome
{
	Block* topLayer{};
	std::vector<BlockLayer> layers{};
	BlockLayer beach{};
	const Structure* bigVegitation{};
	// TODO: Add vegitation structures
};