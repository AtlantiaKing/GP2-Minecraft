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