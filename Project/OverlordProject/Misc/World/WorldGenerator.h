#pragma once

#include "Utils/Perlin.h"
#include "TileAtlas.h"
#include "Chunk.h"

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

	void LoadWorld(std::vector<Chunk>& chunks);
	void RemoveBlock(std::vector<Chunk>& chunks, const XMFLOAT3& position);
	void CreateVertices(const std::vector<Chunk>& chunks, Chunk& chunk);

	void SetRenderDistance(int renderDistance) { m_RenderDistance = renderDistance; }
	void SetWorldHeight(int worldHeight) { m_WorldHeight = worldHeight; }
	void SetTerrainHeight(int terrainHeight) { m_TerrainHeight = terrainHeight; }

	std::vector<XMFLOAT3> GetPositions(const Chunk& chunk) const;
	Chunk& GetWater() { return m_Water; }
private:
	void LoadChunk(std::vector<Chunk>& chunks, int x, int y);

	BlockType GetBlockType(const XMINT3& position, float worldHeight, const Chunk& chunk) const;
	FaceType GetFaceType(BlockType blockType, FaceDirection faceDirection) const;

	std::function<bool(const std::vector<Chunk>& chunks, const XMINT3& position)> m_IsBlockPredicate{};
	std::function<bool(const std::vector<Chunk>& chunks, Block* pBlock, const XMINT3& position)> m_CanRenderPredicate{};
	XMINT3 m_NeighbouringBlocks[6]{};
	std::vector<VertexPosNormTex> m_CubeVertices{};

	Perlin m_UnderSeaPerlin{};
	Perlin m_HeightPerlin{};
	Perlin m_UnderSeaInversedPerlin{};
	TileAtlas m_TileMap{};

	int m_RenderDistance{ 10 };
	int m_WorldHeight{ 256 };
	int m_TerrainHeight{ 128 };
	int m_SeaLevel{ 64 };
	int m_BeachSize{ 2 };
	const int m_ChunkSize{ 16 };

	Chunk m_Water{};
};

