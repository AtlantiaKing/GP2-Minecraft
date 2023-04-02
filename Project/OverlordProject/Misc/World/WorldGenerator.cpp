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
	m_CanRenderPredicate = [&](Block* pBlock, const XMINT3& neighbourPos) -> bool
	{
		const XMINT2 chunkPos{ neighbourPos.x / m_ChunkSize, neighbourPos.z / m_ChunkSize };

		auto it{ std::find_if(begin(m_Chunks), end(m_Chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
			}) };
		if (it == m_Chunks.end()) return false;

		const XMINT3 lookUpPos{ neighbourPos.x - chunkPos.x * m_ChunkSize, neighbourPos.y, neighbourPos.z - chunkPos.y * m_ChunkSize };

		if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
			|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
			|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) return false;

		Block* pNeighbour{ it->pBlocks[lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize] };

		if (!pNeighbour) return true;

		if (pBlock->type != BlockType::WATER && pNeighbour->type == BlockType::WATER)
		{
			if (!m_IsBlockPredicate(XMINT3{ neighbourPos.x, neighbourPos.y + 1, neighbourPos.z })) return true;
		}

		return false;
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

					const XMINT3 position{ chunk.position.x * m_ChunkSize + x, y, chunk.position.y * m_ChunkSize + z };

					for (unsigned int i{}; i <= static_cast<unsigned int>(FaceDirection::BOTTOM); ++i)
					{
						const XMINT3 neightbourDirection{ m_NeighbouringBlocks[i] };
						
						const XMVECTOR positionVector = XMLoadSInt3(&position);
						const XMVECTOR neighbourDirection = XMLoadSInt3(&neightbourDirection);
						XMINT3 neighbourPosition{};
						const XMVECTOR neighbourPosVector = XMVectorAdd(positionVector, neighbourDirection);
						XMStoreSInt3(&neighbourPosition, neighbourPosVector);

						if (!m_CanRenderPredicate(pBlock, neighbourPosition)) continue;

						constexpr int faceIndices[6]{ 0,1,2,3,2,1 };
						for (int vIdx : faceIndices)
						{
							VertexPosNormTex v{ m_CubeVertices[i * 4 + vIdx] };

							if (pBlock->type == BlockType::WATER)
							{
								const float waterDisplacement{ 0.125f };
								v.Position.y = v.Position.y - waterDisplacement;
							}

							XMVECTOR pos{ XMLoadFloat3(&v.Position) };
							pos += XMVECTOR{ static_cast<float>(x),static_cast<float>(y),static_cast<float>(z) } 
									+ XMVECTOR{ static_cast<float>(chunk.position.x * m_ChunkSize), 0.0f, static_cast<float>(chunk.position.y * m_ChunkSize) };
							XMStoreFloat3(&v.Position, pos);

							v.TexCoord = m_TileMap.GetUV(GetFaceType(pBlock->type, static_cast<FaceDirection>(i)), v.TexCoord);

							m_Vertices.push_back(v);
						}
					}
				}
			}
		}
	}

	return m_Vertices;
}

void WorldGenerator::LoadChunk(int chunkX, int chunkY)
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
				Block* pBlock{ new Block{ GetBlockType(XMINT3{x,y,z}, chunk) } };
				
				if (pBlock->type == BlockType::WATER && y == worldY - 1)
				{
					chunk.pBlocks[x + z * m_ChunkSize + (y + 1) * m_ChunkSizeSqr] = pBlock;
				}

				chunk.pBlocks[x + z * m_ChunkSize + y * m_ChunkSizeSqr] = pBlock;
			}
		}
	}

	m_Chunks.push_back(chunk);
}

BlockType WorldGenerator::GetBlockType(const XMINT3& position, const Chunk& chunk) const
{
	if (position.y <= m_SeaLevel) return BlockType::WATER;

	if (position.y <= m_SeaLevel + m_BeachSize) return BlockType::SAND;

	if (!chunk.pBlocks[position.x + position.z * m_ChunkSize + (position.y + 1) * m_ChunkSize * m_ChunkSize]) return BlockType::GRASS;

	return BlockType::DIRT;
}

FaceType WorldGenerator::GetFaceType(BlockType blockType, FaceDirection faceDirection) const
{
	switch (blockType)
	{
	case BlockType::GRASS:
	{
		switch (faceDirection)
		{
		case FaceDirection::UP:
			return FaceType::GRASS;
		case FaceDirection::BOTTOM:
			return FaceType::DIRT;
		default:
			return FaceType::GRASS_SIDE;
		}
	}
	}

	return static_cast<FaceType>(blockType);
}
