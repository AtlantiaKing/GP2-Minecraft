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
	WOOD
};

enum class BlockType : BYTE
{
	DIRT = 0,
	GRASS_BLOCK = 1,
	SAND = 3,
	WATER = 4,
	BEDROCK = 5,
	STONE = 6,
	COBBLESTONE = 7,
	SANDSTONE = 8,
	OAK_LOG = 11,
	OAK_LEAVES = 13,
	GRASS = 14,
	WOOD
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