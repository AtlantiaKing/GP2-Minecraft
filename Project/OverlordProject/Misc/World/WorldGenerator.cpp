#include "stdafx.h"
#include "WorldGenerator.h"
#include "Misc/World/WorldData.h"

WorldGenerator::WorldGenerator()
	: m_HeightPerlin{ 4, 5 }
	, m_UnderSeaPerlin{ 5, 25 }
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

	m_WorldWidth = m_ChunkSize * (renderRadius * 2 + 1);
	m_Water.pBlocks.resize(m_WorldHeight * m_WorldWidth * m_WorldWidth);

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

	CreateWaterVertices();
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

bool WorldGenerator::ChangeEnvironment(std::vector<Chunk>& chunks, const XMINT2& chunkCenter)
{
	bool changedEnvironment{};

	// Start and end point of water blocks that have to be recalculated
	const int minX{ (m_RenderDistance - 2 + chunkCenter.x) * m_ChunkSize };
	const int maxX{ minX + 3 * m_ChunkSize };
	const int minZ{ (m_RenderDistance - 2 + chunkCenter.y) * m_ChunkSize };
	const int maxZ{ minZ + 3 * m_ChunkSize };

	// A list of all the extra blocks that should be added
	std::vector<XMINT3> newBlocks{};
	std::vector<XMINT3> removeBlocks{};

	// For every block in the recalculate range
	for (int x{ minX }; x < maxX; ++x)
	{
		for (int z{ minZ }; z < maxZ; ++z)
		{
			for (int y{ m_WorldHeight - 1 }; y >= 0; --y)
			{
				// If there is no water block on this position, do nothing
				if (!m_Water.pBlocks[x + z * m_WorldWidth + y * m_WorldWidth * m_WorldWidth]) continue;

				// Calculate the position from water space to world space
				const XMINT3 positionWorld
				{
					x - (m_RenderDistance - 1) * m_ChunkSize,
					y,
					z - (m_RenderDistance - 1) * m_ChunkSize
				};
				// If there is a block at this position, continue to the next face of the block
				if (m_IsBlockPredicate(chunks, positionWorld))
				{
					// Add this block to removal
					removeBlocks.push_back({ x,y,z });

					// Keep track that the environment should be reset
					changedEnvironment = true;
					continue;
				}

				// For each side of the water
				for (unsigned int i{}; i <= static_cast<unsigned int>(FaceDirection::BOTTOM); ++i)
				{
					// Ignore the up side of the water block
					if (i == static_cast<unsigned int>(FaceDirection::UP)) continue;

					// Calculate the neighbouring block position
					const XMINT3& neighbourDirection{ m_NeighbouringBlocks[i] };
					const XMINT3 neighbourPosition{ x + neighbourDirection.x, y + neighbourDirection.y, z + neighbourDirection.z };

					// If the neighbouring block is outside recalculate range, continue to the next face of the block
					if (neighbourPosition.x < minX || neighbourPosition.x >= maxX || neighbourPosition.z < minZ || neighbourPosition.z >= maxZ || neighbourPosition.y < 0) continue;

					// If there is already a water block on this position, continue to the next face of the block
					if (m_Water.pBlocks[neighbourPosition.x + neighbourPosition.z * m_WorldWidth + neighbourPosition.y * m_WorldWidth * m_WorldWidth]) continue;

					// Calculate the neighbour position from water space to world space
					const XMINT3 neightbourPositionWorld
					{
						neighbourPosition.x - (m_RenderDistance-1) * m_ChunkSize,
						neighbourPosition.y,
						neighbourPosition.z - (m_RenderDistance-1) * m_ChunkSize
					};
					// If there is a block at this position, continue to the next face of the block
					if (m_IsBlockPredicate(chunks, neightbourPositionWorld)) continue;

					// Add the block to the list of new blocks
					newBlocks.push_back(neighbourPosition);

					// Keep track that the environment should be reset
					changedEnvironment = true;
				}
			}
		}
	}

	// If the environment has been changed
	if (changedEnvironment)
	{
		// Add the new water blocks to the water object
		for (const XMINT3& block : newBlocks)
		{
			m_Water.pBlocks[block.x + block.z * m_WorldWidth + block.y * m_WorldWidth * m_WorldWidth] = m_pWaterBlock.get();
		}
		// remove the blocks from the water object
		for (const XMINT3& block : removeBlocks)
		{
			m_Water.pBlocks[block.x + block.z * m_WorldWidth + block.y * m_WorldWidth * m_WorldWidth] = nullptr;
		}

		// Reload the water vertices
		CreateWaterVertices();
	}

	// Return whether the environment has been changed or not
	return changedEnvironment;
}

void WorldGenerator::CreateVertices(const std::vector<Chunk>& chunks, Chunk& chunk)
{
	chunk.verticesChanged = true;

	std::vector<VertexPosNormTex> vertices{};
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
					const XMINT3& neightbourDirection{ m_NeighbouringBlocks[i] };

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

						XMVECTOR pos{ XMLoadFloat3(&v.Position) };
						pos += XMVECTOR{ static_cast<float>(x),static_cast<float>(y),static_cast<float>(z) }
						+ XMVECTOR{ static_cast<float>(chunk.position.x * m_ChunkSize), 0.0f, static_cast<float>(chunk.position.y * m_ChunkSize) };
						XMStoreFloat3(&v.Position, pos);

						v.TexCoord = m_TileMap.GetUV(GetFaceType(pBlock->type, static_cast<FaceDirection>(i)), v.TexCoord);

						vertices.push_back(v);
					}
				}
			}
		}
	}
	chunk.vertices = std::move(vertices);

	chunk.loadedWater = true;
}

void WorldGenerator::CreateWaterVertices()
{
	// TODO: Make water chunks to speed up vertices calculation

	std::vector<VertexPosNormTex> vertices{};
	for (int x{ 0 }; x < ((m_RenderDistance - 1) * 2 + 1) * m_ChunkSize; ++x)
	{
		for (int z{ 0 }; z < ((m_RenderDistance - 1) * 2 + 1) * m_ChunkSize; ++z)
		{
			for (int y{ m_WorldHeight - 1 }; y >= 0; --y)
			{
				const WaterBlock* block{ m_Water.pBlocks[x + z * m_WorldWidth + y * m_WorldWidth * m_WorldWidth] };

				if (!block) continue;

				bool hasBlockOnTop{};
				hasBlockOnTop = m_Water.pBlocks[x + z * m_WorldWidth + (y+1) * m_WorldWidth * m_WorldWidth];

				for (unsigned int i{}; i <= static_cast<unsigned int>(FaceDirection::BOTTOM); ++i)
				{
					const XMINT3& neighbourDirection{ m_NeighbouringBlocks[i] };
					const XMINT3 neighbourPosition{ x + neighbourDirection.x, y + neighbourDirection.y, z + neighbourDirection.z };

					if (neighbourPosition.x < 0 || neighbourPosition.x >= m_WorldWidth || neighbourPosition.y < 0 || neighbourPosition.y >= m_WorldHeight || neighbourPosition.z < 0 || neighbourPosition.z >= m_WorldWidth) continue;

					if (m_Water.pBlocks[neighbourPosition.x + neighbourPosition.z * m_WorldWidth + neighbourPosition.y * m_WorldWidth * m_WorldWidth]) continue;

					constexpr int faceIndices[6]{ 0,1,2,3,2,1 };
					for (int vIdx : faceIndices)
					{
						VertexPosNormTex v{ m_CubeVertices[i * 4 + vIdx] };

						if (!hasBlockOnTop && v.Position.y > 0.25f)
						{
							constexpr float waterDisplacement{ 0.125f };
							v.Position.y = v.Position.y - waterDisplacement;
						}

						XMVECTOR pos{ XMLoadFloat3(&v.Position) };
						pos += XMVECTOR{ static_cast<float>(x - m_ChunkSize * (m_RenderDistance - 1)),static_cast<float>(y),static_cast<float>(z- m_ChunkSize * (m_RenderDistance - 1)) };
						XMStoreFloat3(&v.Position, pos);

						v.TexCoord = m_TileMap.GetUV(GetFaceType(BlockType::WATER, static_cast<FaceDirection>(i)), v.TexCoord);

						vertices.push_back(v);
					}
				}
			}
		}
	}

	m_Water.vertices = std::move(vertices);
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

			const float underseaNoise{ m_UnderSeaPerlin.GetNoise(static_cast<float>(worldPosX) / m_ChunkSize, static_cast<float>(worldPosZ) / m_ChunkSize) };
			const float seaWorldHeight{ underseaNoise * m_TerrainHeight };

			const float heightNoise{ m_HeightPerlin.GetNoise(static_cast<float>(worldPosX) / m_ChunkSize, static_cast<float>(worldPosZ) / m_ChunkSize) };
			float worldHeight{};

			if (seaWorldHeight < m_SeaLevel)
			{
				worldHeight = seaWorldHeight * 2 - m_SeaLevel;
			}
			else
			{
				const float amountAboveSealevel{ seaWorldHeight - m_SeaLevel };
				const float percentageAboveSealevel{ amountAboveSealevel / (m_TerrainHeight - m_SeaLevel) };

				worldHeight = m_SeaLevel + heightNoise * m_TerrainHeight * percentageAboveSealevel;
			}

			const int worldY = std::min(std::max(static_cast<int>(worldHeight), m_SeaLevel + 1), m_WorldHeight - 1);
			const float beachMultiplier{ m_BeachPerlin.GetNoise(static_cast<float>(worldPosX) / m_ChunkSize, static_cast<float>(worldPosZ) / m_ChunkSize) };
			const float beachSize{ (beachMultiplier * m_BeachSize) };

			bool hasDirt{ false };
			for (int y{ worldY - 1 }; y >= 0; --y)
			{
				const XMINT3 blockPosition{ XMINT3{x,y,z} };
				const BlockType blockType{ GetBlockType(blockPosition, worldHeight, beachSize, chunk) };

				if (blockType == BlockType::WATER)
				{
					const int waterPosX{ worldPosX + m_ChunkSize * (m_RenderDistance - 1) };
					const int waterPosZ{ worldPosZ + m_ChunkSize * (m_RenderDistance - 1) };
					m_Water.pBlocks[waterPosX + waterPosZ * m_WorldWidth + y * m_WorldWidth * m_WorldWidth] = m_pWaterBlock.get();
					continue;
				}

				Block* pBlock{ new Block{ blockType } };

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
