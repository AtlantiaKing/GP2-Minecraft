#include "stdafx.h"
#include "WorldGenerator.h"
#include "Misc/World/WorldData.h"

WorldGenerator::WorldGenerator()
	: m_HeightPerlin{ 2,5 }
	, m_BeachPerlin{ 2, 1 }
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

	m_IsBlockPredicate = [&](const std::vector<Chunk>& chunks, const XMINT3& position) -> bool
	{
		const XMINT2 chunkPos
		{
			position.x < 0 ? (position.x + 1) / m_ChunkSize - 1 : position.x / m_ChunkSize,
			position.z < 0 ? (position.z + 1) / m_ChunkSize - 1 : position.z / m_ChunkSize
		};

		auto it{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
			}) };
		if (it == chunks.end()) return false;

		const XMINT3 lookUpPos{ position.x - chunkPos.x * m_ChunkSize, position.y, position.z - chunkPos.y * m_ChunkSize };

		if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
			|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
			|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) return false;

		return it->pBlocks[lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize];
	};
	m_CanRenderPredicate = [&](const std::vector<Chunk>& chunks, Block* pBlock, const XMINT3& neighbourPos) -> bool
	{
		const XMINT2 chunkPos
		{ 
			neighbourPos.x < 0 ? (neighbourPos.x + 1) / m_ChunkSize - 1 : neighbourPos.x / m_ChunkSize, 
			neighbourPos.z < 0 ? (neighbourPos.z + 1) / m_ChunkSize - 1 : neighbourPos.z / m_ChunkSize
		};

		auto it{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
			}) };
		if (it == chunks.end()) return false;

		const XMINT3 lookUpPos{ neighbourPos.x - chunkPos.x * m_ChunkSize, neighbourPos.y, neighbourPos.z - chunkPos.y * m_ChunkSize };

		if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
			|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
			|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) return false;

		Block* pNeighbour{ it->pBlocks[lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize] };

		if (!pNeighbour) return true;

		if (pBlock->type != BlockType::WATER && pNeighbour->type == BlockType::WATER) return true;

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

void WorldGenerator::LoadWorld(std::vector<Chunk>& chunks)
{
	const int renderRadius{ m_RenderDistance - 1 };

	for (int x{ -renderRadius }; x <= renderRadius; ++x)
	{
		for (int y{ -renderRadius }; y <= renderRadius; ++y)
		{
			LoadChunk(chunks, x, y);
		}
	}

	for (Chunk& chunk : chunks)
	{
		CreateVertices(chunks, chunk);
	}
}

void WorldGenerator::RemoveBlock(std::vector<Chunk>& chunks, const XMFLOAT3& position)
{
	const XMINT2 chunkPos
	{
		position.x < 0 ? (static_cast<int>(position.x) + 1) / m_ChunkSize - 1 : static_cast<int>(position.x) / m_ChunkSize,
		position.z < 0 ? (static_cast<int>(position.z) + 1) / m_ChunkSize - 1 : static_cast<int>(position.z) / m_ChunkSize
	};

	auto it{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
		{
			return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
		}) };
	if (it == chunks.end()) return;

	const XMINT3 lookUpPos{ static_cast<int>(position.x) - chunkPos.x * m_ChunkSize, static_cast<int>(position.y), static_cast<int>(position.z) - chunkPos.y * m_ChunkSize };

	if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
		|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
		|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) return;

	const int blockIdx{ lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize };
	if (!it->pBlocks[blockIdx]) return;

	delete it->pBlocks[blockIdx];
	it->pBlocks[blockIdx] = nullptr;

	CreateVertices(chunks, *it);

	if (lookUpPos.x == 0 || lookUpPos.x == m_ChunkSize - 1)
	{
		const int otherChunkX{ lookUpPos.x == 0 ? chunkPos.x - 1 : chunkPos.x + 1 };

		auto neighbourIt{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == otherChunkX && chunk.position.y == chunkPos.y;
			}) };

		if (neighbourIt != chunks.end())
			CreateVertices(chunks, *neighbourIt);
	}

	if (lookUpPos.z == 0 || lookUpPos.z == m_ChunkSize - 1)
	{
		const int otherChunkY{ lookUpPos.z == 0 ? chunkPos.y - 1 : chunkPos.y + 1 };

		auto neighbourIt{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == chunkPos.x && chunk.position.y == otherChunkY;
			}) };

		if (neighbourIt != chunks.end())
			CreateVertices(chunks, *neighbourIt);
	}
}

void WorldGenerator::PlaceBlock(std::vector<Chunk>& chunks, const XMFLOAT3& position, BlockType block)
{
	const XMINT2 chunkPos
	{
		position.x < 0 ? (static_cast<int>(position.x) + 1) / m_ChunkSize - 1 : static_cast<int>(position.x) / m_ChunkSize,
		position.z < 0 ? (static_cast<int>(position.z) + 1) / m_ChunkSize - 1 : static_cast<int>(position.z) / m_ChunkSize
	};

	auto it{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
		{
			return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
		}) };
	if (it == chunks.end()) return;

	const XMINT3 lookUpPos{ static_cast<int>(position.x) - chunkPos.x * m_ChunkSize, static_cast<int>(position.y), static_cast<int>(position.z) - chunkPos.y * m_ChunkSize };

	if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
		|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
		|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) return;

	const int blockIdx{ lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize };
	if (it->pBlocks[blockIdx]) return;

	it->pBlocks[blockIdx] = new Block{ block };

	CreateVertices(chunks, *it);

	if (lookUpPos.x == 0 || lookUpPos.x == m_ChunkSize - 1)
	{
		const int otherChunkX{ lookUpPos.x == 0 ? chunkPos.x - 1 : chunkPos.x + 1 };

		auto neighbourIt{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == otherChunkX && chunk.position.y == chunkPos.y;
			}) };

		if (neighbourIt != chunks.end())
			CreateVertices(chunks, *neighbourIt);
	}

	if (lookUpPos.z == 0 || lookUpPos.z == m_ChunkSize - 1)
	{
		const int otherChunkY{ lookUpPos.z == 0 ? chunkPos.y - 1 : chunkPos.y + 1 };

		auto neighbourIt{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == chunkPos.x && chunk.position.y == otherChunkY;
			}) };

		if (neighbourIt != chunks.end())
			CreateVertices(chunks, *neighbourIt);
	}
}

void WorldGenerator::CreateVertices(const std::vector<Chunk>& chunks, Chunk& chunk)
{
	chunk.verticesChanged = true;

	chunk.vertices.clear();

	for (int x{}; x < m_ChunkSize; ++x)
	{
		for (int z{}; z < m_ChunkSize; ++z)
		{
			for (int y{ m_WorldHeight - 1 }; y >= 0; --y)
			{
				Block* pBlock{ chunk.pBlocks[x + z * m_ChunkSize + y * m_ChunkSize * m_ChunkSize] };

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

					if (!m_CanRenderPredicate(chunks, pBlock, neighbourPosition)) continue;

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

						if (pBlock->type == BlockType::WATER)
						{
							if(!chunk.loadedWater) m_Water.vertices.push_back(v);
						}
						else
						{
							chunk.vertices.push_back(v);
						}
					}
				}
			}
		}
	}

	chunk.loadedWater = true;
}

std::vector<XMFLOAT3> WorldGenerator::GetPositions(const Chunk& chunk) const
{
	std::vector<XMFLOAT3> vertices{};
	vertices.reserve(chunk.vertices.size());

	int i{};
	for (const VertexPosNormTex& v : chunk.vertices)
	{
		vertices.emplace_back(v.Position);
		++i;
		if (i >= chunk.vertices.size()) break;
	}

	return vertices;
}

void WorldGenerator::LoadChunk(std::vector<Chunk>& chunks, int chunkX, int chunkY)
{
	const int m_ChunkSizeSqr{ m_ChunkSize * m_ChunkSize };

	Chunk chunk{};
	chunk.pBlocks.resize(m_WorldHeight * m_ChunkSize * m_ChunkSize);
	chunk.position.x = chunkX;
	chunk.position.y = chunkY;

	for (int x{}; x < m_ChunkSize; ++x)
	{
		for (int z{}; z < m_ChunkSize; ++z)
		{
			const int worldPosX{ chunkX * m_ChunkSize + x };
			const int worldPosZ{ chunkY * m_ChunkSize + z };

			const float UnderseaNoise{ m_UnderSeaPerlin.GetNoise(static_cast<float>(worldPosX) / m_ChunkSize, static_cast<float>(worldPosZ) / m_ChunkSize) };
			const float heightNoise{ m_HeightPerlin.GetNoise(static_cast<float>(worldPosX) / m_ChunkSize, static_cast<float>(worldPosZ) / m_ChunkSize) };
			const float beachMultiplier{ m_BeachPerlin.GetNoise(static_cast<float>(worldPosX) / m_ChunkSize, static_cast<float>(worldPosZ) / m_ChunkSize) };
			
			float worldHeight{};
			const float beachSize{ (beachMultiplier * m_BeachSize) };

			if (UnderseaNoise < static_cast<float>(m_SeaLevel) / m_TerrainHeight)
			{
				worldHeight = UnderseaNoise * m_TerrainHeight;
			}
			else
			{
				const float inversedSeaLevel{ UnderseaNoise * m_TerrainHeight - m_SeaLevel };
				const bool canAddHeightMap{ inversedSeaLevel > beachSize + 1 };
				const float landBase{ canAddHeightMap ? beachSize : inversedSeaLevel };
				worldHeight = m_SeaLevel + landBase;
				if (canAddHeightMap)
				{
					worldHeight += heightNoise * (m_TerrainHeight - m_SeaLevel) * inversedSeaLevel / (m_TerrainHeight - m_SeaLevel);
				}
			}

			const int worldY = std::min(std::max(static_cast<int>(worldHeight), m_SeaLevel + 1), m_WorldHeight - 1);

			bool hasDirt{ false };
			for (int y{ worldY - 1 }; y >= 0; --y)
			{
				Block* pBlock{ new Block{ GetBlockType(XMINT3{x,y,z}, worldHeight, beachSize, chunk) } };

				if (pBlock->type == BlockType::DIRT || pBlock->type == BlockType::GRASS) hasDirt = true;
				if (pBlock->type == BlockType::SAND && hasDirt) pBlock->type = BlockType::DIRT;

				chunk.pBlocks[x + z * m_ChunkSize + y * m_ChunkSizeSqr] = pBlock;
			}
		}
	}

	chunks.push_back(chunk);
}

BlockType WorldGenerator::GetBlockType(const XMINT3& position, float worldHeight, float beachSize, const Chunk& chunk) const
{
	if (position.y == 0) return BlockType::BEDROCK;

	if (position.y <= m_SeaLevel && position.y > worldHeight) return BlockType::WATER;

	if (position.y <= m_SeaLevel + beachSize) return BlockType::SAND;

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
