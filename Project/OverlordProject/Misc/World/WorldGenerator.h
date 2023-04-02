#pragma once

#include "Utils/Perlin.h"
#include "TileAtlas.h"
#include <vector>

struct Block;

class WorldGenerator final
{
public:
	WorldGenerator();
	~WorldGenerator() = default;

	WorldGenerator(const WorldGenerator& other) = delete;
	WorldGenerator(WorldGenerator&& other) noexcept = delete;
	WorldGenerator& operator=(const WorldGenerator& other) = delete;
	WorldGenerator& operator=(WorldGenerator&& other) noexcept = delete;

	const std::vector<VertexPosNormTex>& LoadWorld();
	void SetRenderDistance(int renderDistance) { m_RenderDistance = renderDistance; }
	void SetWorldHeight(int worldHeight) { m_WorldHeight = worldHeight; }

	void LoadChunk(int x, int y);
private:
	struct Chunk
	{
		XMINT2 position;
		Block** pBlocks{};
	};

	BlockType GetBlockType(const XMINT3& position, const Chunk& chunk) const;
	FaceType GetFaceType(BlockType blockType, FaceDirection faceDirection) const;

	std::function<bool(const XMINT3& position)> m_IsBlockPredicate{};
	std::function<bool(Block* pBlock, const XMINT3& position)> m_CanRenderPredicate{};
	XMINT3 m_NeighbouringBlocks[6]{};
	std::vector<VertexPosNormTex> m_CubeVertices{};

	Perlin m_Perlin{};
	TileAtlas m_TileMap{};

	int m_RenderDistance{ 5 };
	int m_WorldHeight{ 64 };
	int m_SeaLevel{ 30 };
	int m_BeachSize{ 2 };
	const int m_ChunkSize{ 16 };

	std::vector<VertexPosNormTex> m_Vertices{};
	std::vector<Chunk> m_Chunks{};
};

