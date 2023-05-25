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
	AIR,
	DIRT,
	GRASS_BLOCK,
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
	OAK_LEAVES,
	GRASS,
	WOOL
};

enum class BlockType : BYTE
{
	AIR = 0,
	DIRT = 1,
	GRASS_BLOCK = 2,
	SAND = 4,
	WATER = 5,
	BEDROCK = 6,
	STONE = 7,
	COBBLESTONE = 8,
	SANDSTONE = 9,
	OAK_LOG = 12,
	OAK_LEAVES = 14,
	GRASS = 15,
	WOOL = 16
};

enum class BlockMesh : BYTE
{
	CUBE,
	CROSS
};

struct Block
{
	BlockType type{};
	BlockMesh mesh{};
	Block* dropBlock{};
	float breakTime{};
	bool transparent{};
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
	const Structure* smallVegitation{};
};