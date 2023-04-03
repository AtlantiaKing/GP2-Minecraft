#pragma once

#include "Utils/Perlin.h"
#include "TileAtlas.h"
#include <vector>

struct Block;

class WorldGenerator final
{
public:
	WorldGenerator();
	~WorldGenerator();

	WorldGenerator(const WorldGenerator& other) = delete;
	WorldGenerator(WorldGenerator&& other) noexcept = delete;
	WorldGenerator& operator=(const WorldGenerator& other) = delete;
	WorldGenerator& operator=(WorldGenerator&& other) noexcept = delete;

	const std::vector<VertexPosNormTex>& LoadWorld();
	std::vector<XMFLOAT3> GetVertices();
	void SetRenderDistance(int renderDistance) { m_RenderDistance = renderDistance; }
	void SetWorldHeight(int worldHeight) { m_WorldHeight = worldHeight; }
	void SetTerrainHeight(int terrainHeight) { m_TerrainHeight = terrainHeight; }

	void LoadChunk(int x, int y);
	int GetNrWaterVertices() { return m_NrWaterVertices; }
private:
	struct Chunk
	{
		XMINT2 position;
		std::vector<Block*> pBlocks{};
	};

	BlockType GetBlockType(const XMINT3& position, float worldHeight, const Chunk& chunk) const;
	FaceType GetFaceType(BlockType blockType, FaceDirection faceDirection) const;

	std::function<bool(const XMINT3& position)> m_IsBlockPredicate{};
	std::function<bool(Block* pBlock, const XMINT3& position)> m_CanRenderPredicate{};
	XMINT3 m_NeighbouringBlocks[6]{};
	std::vector<VertexPosNormTex> m_CubeVertices{};

	Perlin m_UnderSeaPerlin{};
	Perlin m_HeightPerlin{};
	Perlin m_UnderSeaInversedPerlin{};
	TileAtlas m_TileMap{};

	int m_RenderDistance{ 16 };
	int m_WorldHeight{ 256 };
	int m_TerrainHeight{ 128 };
	int m_SeaLevel{ 64 };
	int m_BeachSize{ 2 };
	const int m_ChunkSize{ 16 };

	int m_NrWaterVertices{};

	std::vector<VertexPosNormTex> m_Vertices{};
	std::vector<Chunk> m_Chunks{};
};

