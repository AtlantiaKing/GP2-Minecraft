#pragma once

#include "Utils/Perlin.h"
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

	const std::vector<VertexPosNormCol>& LoadWorld();
	void SetRenderDistance(int renderDistance) { m_RenderDistance = renderDistance; }
	void SetWorldHeight(int worldHeight) { m_WorldHeight = worldHeight; }

	void LoadChunk(int x, int y/*, ID3D11Device* pDevice*/);
private:
	std::function<bool(const XMINT3& position)> m_IsBlockPredicate{};
	XMINT3 m_NeighbouringBlocks[6]{};
	std::vector<VertexPosNormCol> m_CubeVertices{};

	struct Chunk
	{
		XMINT2 position;
		Block** pBlocks{};
	};

	Perlin m_Perlin{};

	int m_RenderDistance{ 5 };
	int m_WorldHeight{ 64 };
	int m_SeaLevel{ 30 };
	const int m_ChunkSize{ 16 };

	std::vector<VertexPosNormCol> m_Vertices{};
	std::vector<Chunk> m_Chunks{};
};

