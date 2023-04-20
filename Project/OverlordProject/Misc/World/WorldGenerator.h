#pragma once

#include "Utils/Perlin.h"
#include "TileAtlas.h"
#include "Chunk.h"

#include <vector>
#include "Water.h"

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
	void PlaceBlock(std::vector<Chunk>& chunks, const XMFLOAT3& position, BlockType block);
	bool ChangeEnvironment(std::vector<Chunk>& chunks, const XMINT2& chunkCenter);

	void SetRenderDistance(int renderDistance) { m_RenderDistance = renderDistance; }
	void SetWorldHeight(int worldHeight) { m_WorldHeight = worldHeight; }
	void SetTerrainHeight(int terrainHeight) { m_TerrainHeight = terrainHeight; }

	std::vector<XMFLOAT3> GetPositions(const Chunk& chunk) const;
	Water& GetWater() { return m_Water; }
	int GetChunkSize() const { return m_ChunkSize; }
private:
	void LoadChunk(std::vector<Chunk>& chunks, int x, int y);
	void CreateVertices(const std::vector<Chunk>& chunks, Chunk& chunk);
	void CreateWaterVertices();

	BlockType GetBlockType(const XMINT3& position, float worldHeight, float beachHeight, const Chunk& chunk) const;
	FaceType GetFaceType(BlockType blockType, FaceDirection faceDirection) const;

	std::function<bool(const std::vector<Chunk>& chunks, const XMINT3& position)> m_IsBlockPredicate{};
	std::function<bool(const std::vector<Chunk>& chunks, Block* pBlock, const XMINT3& position)> m_CanRenderPredicate{};
	XMINT3 m_NeighbouringBlocks[6]{};
	std::vector<VertexPosNormTex> m_CubeVertices{};

	Perlin m_UnderSeaPerlin{};
	Perlin m_HeightPerlin{};
	Perlin m_UnderSeaInversedPerlin{};
	Perlin m_BeachPerlin{};
	TileAtlas m_TileMap{};

	int m_RenderDistance{ 10 };
	int m_WorldHeight{ 256 };
	int m_TerrainHeight{ 128 };
	int m_SeaLevel{ 64 };
	int m_BeachSize{ 7 };
	const int m_ChunkSize{ 16 };

	std::unique_ptr<WaterBlock> m_pWaterBlock{ std::make_unique<WaterBlock>() };
	Water m_Water{};
	int m_WorldWidth{};
};

