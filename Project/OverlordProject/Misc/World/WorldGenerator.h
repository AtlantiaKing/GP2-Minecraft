#pragma once

#include "Utils/Perlin.h"
#include "TileAtlas.h"
#include "Chunk.h"

#include <vector>

struct Block;

class WorldRenderer;

class WorldGenerator final
{
public:
	WorldGenerator();
	~WorldGenerator() = default;

	WorldGenerator(const WorldGenerator& other) = delete;
	WorldGenerator(WorldGenerator&& other) noexcept = delete;
	WorldGenerator& operator=(const WorldGenerator& other) = delete;
	WorldGenerator& operator=(WorldGenerator&& other) noexcept = delete;

	bool LoadChunk(const XMINT2& chunkCenter, const SceneContext& sceneContext, WorldRenderer* pRenderer);
	void RemoveBlock(const XMFLOAT3& position, const SceneContext& sceneContext, WorldRenderer* pRenderer);
	void PlaceBlock(const XMFLOAT3& position, BlockType block, const SceneContext& sceneContext, WorldRenderer* pRenderer);
	Block* GetBlockAt(int x, int y, int z) const;
	bool ChangeEnvironment(const XMINT2& chunkCenter);

	void SetRenderDistance(int renderDistance) { m_RenderDistance = renderDistance; }
	void SetWorldHeight(int worldHeight) { m_WorldHeight = worldHeight; }
	void SetTerrainHeight(int terrainHeight) { m_TerrainHeight = terrainHeight; }

	std::vector<XMFLOAT3> GetPositions(const Chunk& chunk) const;
	int GetChunkSize() const { return m_ChunkSize; }
	int GetWorldHeight() const { return m_WorldHeight; }

	std::vector<Chunk>& GetChunks() { return m_Chunks; }
	std::vector<Chunk>& GetWater() { return m_WaterChunks; }
private:
	BlockType* GetBlockInChunk(int x, int y, int z, std::vector<Chunk>& chunks) const;
	BlockType const* GetBlockInChunk(int x, int y, int z, const std::vector<Chunk>& chunks) const;
	Chunk* GetChunkAt(int x, int z, std::vector<Chunk>& chunks) const;

	void LoadChunk(int x, int y);
	void ReloadChunks(int changedX, int changedY, int changedZ);
	void ReloadChunks(int chunkX, int chunkY);
	void SpawnStructure(const Structure* structure, const XMINT3& position);
	void CreateVertices(Chunk& chunk, const std::vector<std::vector<Chunk>*>& predicateChunks);

	void CreateVerticesCube(Chunk& chunk, int x, int y, int z, const std::vector<std::vector<Chunk>*>& predicateChunks, Block* pBlock, std::vector<VertexPosNormTexTransparency>& vertices);
	void CreateVerticesCross(Chunk& chunk, int x, int y, int z, Block* pBlock, std::vector<VertexPosNormTexTransparency>& vertices);

	Block* GetBlock(const XMINT3& position, float worldHeight, int surfaceY, float beachHeight, const Biome& biome) const;

	std::function<bool(const std::vector<Chunk>& chunks, const XMINT3& position)> m_IsBlockPredicate{};
	std::function<bool(const std::vector<Chunk>& chunks, const XMINT3& position, BlockType curBlock)> m_CanRenderPredicate{};
	XMINT3 m_NeighbouringBlocks[6]{};

	Perlin m_UnderSeaPerlin{};
	Perlin m_HeightPerlin{};
	Perlin m_BeachPerlin{};
	Perlin m_VegitationPerlin{};
	TileAtlas m_TileMap{};

	std::vector<Chunk> m_Chunks{};
	std::vector<Chunk> m_WaterChunks{};

#ifdef _DEBUG
	int m_RenderDistance{ 2 };
#else
	int m_RenderDistance{ 8 };
#endif
	int m_PhysicsDistance{ 2 };
	int m_WorldHeight{ 256 };
	int m_TerrainHeight{ 128 };
	int m_SeaLevel{ 64 };
	const int m_ChunkSize{ 16 };

	std::unique_ptr<Block> m_pWaterBlock{};
	std::vector<std::pair<const Structure*, XMINT3>> m_StructuresToSpawn{};
	int m_WorldWidth{};
};

