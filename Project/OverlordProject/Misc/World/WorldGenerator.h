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
	void PlaceBlock(std::vector<Chunk>& chunks, const XMFLOAT3& position, BlockType block);
	Block* GetBlockAt(int x, int y, int z, const std::vector<Chunk>& chunks);
	bool ChangeEnvironment(std::vector<Chunk>& chunks, const XMINT2& chunkCenter);

	void SetRenderDistance(int renderDistance) { m_RenderDistance = renderDistance; }
	void SetWorldHeight(int worldHeight) { m_WorldHeight = worldHeight; }
	void SetTerrainHeight(int terrainHeight) { m_TerrainHeight = terrainHeight; }

	std::vector<XMFLOAT3> GetPositions(const Chunk& chunk) const;
	std::vector<Chunk>& GetWater() { return m_WaterChunks; }
	int GetChunkSize() const { return m_ChunkSize; }
private:
	Block** GetBlockInChunk(int x, int y, int z, std::vector<Chunk>& chunks) const;
	Block* const* GetBlockInChunk(int x, int y, int z, const std::vector<Chunk>& chunks) const;
	Chunk* GetChunkAt(int x, int z, std::vector<Chunk>& chunks);

	void LoadChunk(std::vector<Chunk>& chunks, int x, int y);
	void ReloadChunks(std::vector<Chunk>& chunks, int changedX, int changedY, int changedZ);
	void SpawnStructure(std::vector<Chunk>& chunks, const Structure* structure, const XMINT3& position);
	void CreateVertices(const std::vector<Chunk>& chunks, Chunk& chunk, const std::vector<std::vector<Chunk>*>& predicateChunks);

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

	int m_RenderDistance{ 5 };
	int m_PhysicsDistance{ 2 };
	int m_WorldHeight{ 256 };
	int m_TerrainHeight{ 128 };
	int m_SeaLevel{ 64 };
	const int m_ChunkSize{ 16 };

	std::unique_ptr<Block> m_pWaterBlock{};
	std::vector<Chunk> m_WaterChunks{};
	std::vector<std::pair<const Structure*, XMINT3>> m_StructuresToSpawn{};
	int m_WorldWidth{};
};

