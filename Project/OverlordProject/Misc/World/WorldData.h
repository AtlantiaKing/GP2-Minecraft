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
	WATER
};

enum class BlockType : BYTE
{
	DIRT,
	GRASS,
	GRASS_SIDE, // NOT USED, JUST PADDING
	SAND,
	WATER
};

struct Block
{
	BlockType type{};
};