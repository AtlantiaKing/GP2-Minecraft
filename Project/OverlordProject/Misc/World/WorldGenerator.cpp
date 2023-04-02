#include "stdafx.h"
#include "WorldGenerator.h"
#include "Misc/World/WorldData.h"

WorldGenerator::WorldGenerator()
{
	m_CubeVertices =
	{
			{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0, 0.0f } },
			{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0, 1.0 } },
			{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },
			{ { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0 } },

			{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0, 0.0f } },
			{ { 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0, 1.0 } },
			{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f } },
			{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0 } },

			{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0, 0.0f } },
			{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0, 1.0 } },
			{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
			{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0 } },

			{ { -0.5f, 0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0, 0.0f } },
			{ { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0, 1.0 } },
			{ { -0.5f, 0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0 } },

			{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0, 0.0f } },
			{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0, 1.0 } },
			{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
			{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0 } },

			{ { 0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0, 0.0f } },
			{ { 0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0, 1.0 } },
			{ { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f } },
			{ { -0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0 } }
	};

	m_IsBlockPredicate = [&](const XMINT3& position) -> bool
	{
		const XMINT2 chunkPos{ position.x / m_ChunkSize, position.z / m_ChunkSize };

		auto it{ std::find_if(begin(m_Chunks), end(m_Chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
			}) };
		if (it == m_Chunks.end()) return false;

		const XMINT3 lookUpPos{ position.x - chunkPos.x * m_ChunkSize, position.y, position.z - chunkPos.y * m_ChunkSize };

		if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
			|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
			|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) return false;

		return it->pBlocks[lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize];
	};

	for (int i{}; i <= static_cast<int>(FaceDirection::BOTTOM); ++i)
	{
		switch (static_cast<FaceDirection>(i))
		{
		case FaceDirection::FORWARD:
		{
			m_NeighbouringBlocks[i] = XMINT3{ 0, 0, 1 };
			break;
		}
		case FaceDirection::BACK:
		{
			m_NeighbouringBlocks[i] = XMINT3{ 0, 0, -1 };
			break;
		}
		case FaceDirection::RIGHT:
		{
			m_NeighbouringBlocks[i] = XMINT3{ 1, 0, 0 };
			break;
		}
		case FaceDirection::LEFT:
		{
			m_NeighbouringBlocks[i] = XMINT3{ -1, 0, 0 };
			break;
		}
		case FaceDirection::UP:
		{
			m_NeighbouringBlocks[i] = XMINT3{ 0, 1, 0 };
			break;
		}
		case FaceDirection::BOTTOM:
		{
			m_NeighbouringBlocks[i] = XMINT3{ 0, -1, 0 };
			break;
		}
		}
	}
}

const std::vector<VertexPosNormTex>& WorldGenerator::LoadWorld()
{
	const int renderRadius{ m_RenderDistance - 1 };

	for (int x{ 0 }; x <= renderRadius * 2; ++x)
	{
		for (int y{ 0 }; y <= renderRadius * 2; ++y)
		{
			LoadChunk(x, y);
		}
	}

	m_Vertices.clear();

	const int m_ChunkSizeSqr{ m_ChunkSize * m_ChunkSize };

	for (const Chunk& chunk : m_Chunks)
	{
		for (int x{}; x < m_ChunkSize; ++x)
		{
			for (int z{}; z < m_ChunkSize; ++z)
			{
				for (int y{ m_WorldHeight - 1 }; y >= m_SeaLevel; --y)
				{
					Block* pBlock{ chunk.pBlocks[x + z * m_ChunkSize + y * m_ChunkSizeSqr] };

					if (!pBlock) continue;

					for (unsigned int i{}; i <= static_cast<unsigned int>(FaceDirection::BOTTOM); ++i)
					{
						const XMINT3 position{ chunk.position.x * m_ChunkSize + x, y, chunk.position.y * m_ChunkSize + z };
						const XMINT3 neightbourDirection{ m_NeighbouringBlocks[i] };
						
						const XMVECTOR v1 = XMLoadSInt3(&position);
						const XMVECTOR v2 = XMLoadSInt3(&neightbourDirection);
						XMINT3 result{};
						const XMVECTOR vResult = XMVectorAdd(v1, v2);
						XMStoreSInt3(&result, vResult);

						if (m_IsBlockPredicate(result)) continue;

						const std::vector<int> faceIndices{ 0,1,2,3,2,1 };
						for (int vIdx : faceIndices)
						{
							VertexPosNormTex v{ m_CubeVertices[i * 4 + vIdx] };

							XMVECTOR pos{ XMLoadFloat3(&v.Position) };
							pos += XMVECTOR{ static_cast<float>(x),static_cast<float>(y),static_cast<float>(z) } 
									+ XMVECTOR{ static_cast<float>(chunk.position.x * m_ChunkSize), 0.0f, static_cast<float>(chunk.position.y * m_ChunkSize) };
							XMStoreFloat3(&v.Position, pos);

							v.TexCoord = m_TileMap.GetUV(GetBlockType(static_cast<FaceDirection>(i), XMINT3{x,y,z}, chunk), v.TexCoord);

							m_Vertices.push_back(v);
						}
					}
				}
			}
		}
	}

	return m_Vertices;
}

void WorldGenerator::LoadChunk(int chunkX, int chunkY/*, ID3D11Device* pDevice*/)
{
	const int m_ChunkSizeSqr{ m_ChunkSize * m_ChunkSize };

	Chunk chunk{};
	chunk.pBlocks = new Block* [m_WorldHeight * m_ChunkSize * m_ChunkSize] {};
	chunk.position.x = chunkX;
	chunk.position.y = chunkY;

	for (int x{}; x < m_ChunkSize; ++x)
	{
		for (int z{}; z < m_ChunkSize; ++z)
		{
			const int worldPosX{ chunkX * m_ChunkSize + x };
			const int worldPosZ{ chunkY * m_ChunkSize + z };

			float worldHeight{ m_Perlin.GetNoise(static_cast<float>(worldPosX) / m_ChunkSize, static_cast<float>(worldPosZ) / m_ChunkSize) };
			worldHeight *= m_WorldHeight;

			const int worldY = std::max(static_cast<int>(worldHeight), m_SeaLevel + 1);

			for (int y{ worldY - 1 }; y >= 0; --y)
			{
				chunk.pBlocks[x + z * m_ChunkSize + y * m_ChunkSizeSqr] = new Block{};
			}
		}
	}

	m_Chunks.push_back(chunk);
}

BlockType WorldGenerator::GetBlockType(FaceDirection faceDirection, const XMINT3& position, const Chunk& chunk) const
{
	if (position.y <= m_SeaLevel) return BlockType::WATER;

	if (position.y <= m_SeaLevel + m_BeachSize) return BlockType::SAND;

	if (!chunk.pBlocks[position.x + position.z * m_ChunkSize + (position.y + 1) * m_ChunkSize * m_ChunkSize])
	{
		switch (faceDirection)
		{
		case FaceDirection::UP:
			return BlockType::GRASS;
		case FaceDirection::BOTTOM:
			return BlockType::DIRT;
		default:
			return BlockType::GRASS_SIDE;
		}
	}

	return BlockType::DIRT;
}
