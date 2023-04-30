#include "stdafx.h"
#include "WorldGenerator.h"

#include "Misc/World/WorldData.h"
#include "Managers/BlockManager.h"

WorldGenerator::WorldGenerator()
	: m_HeightPerlin{ 4, 5 }
	, m_UnderSeaPerlin{ 5, 25 }
	, m_BeachPerlin{ 2, 1 }
	, m_BigVegitationPerlin{ 5, 0.1f }
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

	for (int x{ -renderRadius }; x <= renderRadius; ++x)
	{
		for (int y{ -renderRadius }; y <= renderRadius; ++y)
		{
			LoadChunk(chunks, x, y);
		}
	}

	for (const auto& structure : m_StructuresToSpawn)
	{
		SpawnStructure(chunks, structure.first, structure.second);
	}
	m_StructuresToSpawn.clear();


	for (Chunk& chunk : chunks)
	{
		CreateVertices(chunks, chunk);
	}
	for (Chunk& chunk : m_WaterChunks)
	{
		CreateVertices(m_WaterChunks, chunk);
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

	it->pBlocks[blockIdx] = BlockManager::Get()->GetBlock(block);

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

Block* WorldGenerator::GetBlockAt(int x, int y, int z, const std::vector<Chunk>& chunks) const
{
	const XMINT2 chunkPos
	{
		x < 0 ? (static_cast<int>(x) + 1) / m_ChunkSize - 1 : static_cast<int>(x) / m_ChunkSize,
		z < 0 ? (static_cast<int>(z) + 1) / m_ChunkSize - 1 : static_cast<int>(z) / m_ChunkSize
	};

	auto it{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
		{
			return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
		}) };
	if (it == chunks.end()) return nullptr;

	const XMINT3 lookUpPos{ static_cast<int>(x) - chunkPos.x * m_ChunkSize, static_cast<int>(y), static_cast<int>(z) - chunkPos.y * m_ChunkSize };

	if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
		|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
		|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) return nullptr;

	const int blockIdx{ lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize };
	return it->pBlocks[blockIdx];
}

bool WorldGenerator::ChangeEnvironment(std::vector<Chunk>& chunks, const XMINT2& chunkCenter)
{
	bool changedEnvironment{};

	// Start and end point of water blocks that have to be recalculated
	const int minX{ (chunkCenter.x - m_PhysicsDistance) * m_ChunkSize };
	const int maxX{ (chunkCenter.x + m_PhysicsDistance) * m_ChunkSize };
	const int minZ{ (chunkCenter.y - m_PhysicsDistance) * m_ChunkSize };
	const int maxZ{ (chunkCenter.y + m_PhysicsDistance) * m_ChunkSize };

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
				const XMINT3 position{ x,y,z };

				// If there is no water block on this position, do nothing
				if (!m_IsBlockPredicate(m_WaterChunks, position)) continue;

				// If there is a block at this position, continue to the next face of the block
				if (m_IsBlockPredicate(chunks, position))
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
					if (m_IsBlockPredicate(m_WaterChunks, neighbourPosition)) continue;

					// If there is a block at this position, continue to the next face of the block
					if (m_IsBlockPredicate(chunks, neighbourPosition)) continue;

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
		std::vector<std::vector<Chunk>::iterator> chunksThatNeedUpdate{};

		// Add the new water blocks to the water object
		for (const XMINT3& block : newBlocks)
		{
			const XMINT2 chunkPos
			{
				block.x < 0 ? (static_cast<int>(block.x) + 1) / m_ChunkSize - 1 : static_cast<int>(block.x) / m_ChunkSize,
				block.z < 0 ? (static_cast<int>(block.z) + 1) / m_ChunkSize - 1 : static_cast<int>(block.z) / m_ChunkSize
			};

			auto it{ std::find_if(begin(m_WaterChunks), end(m_WaterChunks), [&](const Chunk& chunk)
				{
					return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
				}) };
			if (it == m_WaterChunks.end()) continue;

			const XMINT3 lookUpPos{ static_cast<int>(block.x) - chunkPos.x * m_ChunkSize, static_cast<int>(block.y), static_cast<int>(block.z) - chunkPos.y * m_ChunkSize };

			if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
				|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
				|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) continue;

			if (std::find(begin(chunksThatNeedUpdate), end(chunksThatNeedUpdate), it) == end(chunksThatNeedUpdate)) chunksThatNeedUpdate.push_back(it);

			const int blockIdx{ lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize };
			it->pBlocks[blockIdx] = m_pWaterBlock.get();
		}

		// remove the blocks from the water object
		for (const XMINT3& block : removeBlocks)
		{
			const XMINT2 chunkPos
			{
				block.x < 0 ? (static_cast<int>(block.x) + 1) / m_ChunkSize - 1 : static_cast<int>(block.x) / m_ChunkSize,
				block.z < 0 ? (static_cast<int>(block.z) + 1) / m_ChunkSize - 1 : static_cast<int>(block.z) / m_ChunkSize
			};

			auto it{ std::find_if(begin(m_WaterChunks), end(m_WaterChunks), [&](const Chunk& chunk)
				{
					return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
				}) };
			if (it == m_WaterChunks.end()) continue;

			const XMINT3 lookUpPos{ static_cast<int>(block.x) - chunkPos.x * m_ChunkSize, static_cast<int>(block.y), static_cast<int>(block.z) - chunkPos.y * m_ChunkSize };

			if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
				|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
				|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) continue;

			if (std::find(begin(chunksThatNeedUpdate), end(chunksThatNeedUpdate), it) == end(chunksThatNeedUpdate)) chunksThatNeedUpdate.push_back(it);

			const int blockIdx{ lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize };
			it->pBlocks[blockIdx] = nullptr;
		}

		// Reload the water vertices
		for (const auto& chunk : chunksThatNeedUpdate)
		{
			CreateVertices(m_WaterChunks, *chunk);
		}
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

						if (pBlock->type == BlockType::WATER && !m_IsBlockPredicate(m_WaterChunks, {x,y+1,z}))
						{
							constexpr float waterOffset{ 0.125 };
							if (v.Position.y > 0.0f) v.Position.y -= waterOffset;
						}

						XMVECTOR pos{ XMLoadFloat3(&v.Position) };
						pos += XMVECTOR{ static_cast<float>(x),static_cast<float>(y),static_cast<float>(z) }
						+ XMVECTOR{ static_cast<float>(chunk.position.x * m_ChunkSize), 0.0f, static_cast<float>(chunk.position.y * m_ChunkSize) };
						XMStoreFloat3(&v.Position, pos);

						v.TexCoord = m_TileMap.GetUV(m_TileMap.GetFaceType(pBlock->type, static_cast<FaceDirection>(i)), v.TexCoord);

						vertices.push_back(v);
					}
				}
			}
		}
	}
	chunk.vertices = std::move(vertices);

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
	Biome biome{ BlockManager::Get()->GetBiome("forest") };

	const int m_ChunkSizeSqr{ m_ChunkSize * m_ChunkSize };

	Chunk chunk{};
	chunk.pBlocks.resize(m_WorldHeight * m_ChunkSize * m_ChunkSize);
	chunk.position.x = chunkX;
	chunk.position.y = chunkY;
	Chunk waterChunk{};
	waterChunk.pBlocks.resize(m_WorldHeight * m_ChunkSize * m_ChunkSize);
	waterChunk.position.x = chunkX;
	waterChunk.position.y = chunkY;

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
			const float beachSize{ (beachMultiplier * biome.beach.size) };

			bool hasDirt{ false };

			const int surfaceY{ worldY - 1 };
			for (int y{ surfaceY }; y >= 0; --y)
			{
				const XMINT3 blockPosition{ XMINT3{x,y,z} };
				Block* pBlock{ GetBlock(blockPosition, worldHeight, surfaceY, beachSize, biome) };

				if (pBlock->type == BlockType::WATER)
				{
					waterChunk.pBlocks[x + z * m_ChunkSize + y * m_ChunkSizeSqr] = pBlock;
					continue;
				}

				if (pBlock->type == BlockType::DIRT || pBlock->type == BlockType::GRASS) hasDirt = true;
				if (pBlock->type == BlockType::SAND && hasDirt) pBlock = BlockManager::Get()->GetBlock(BlockType::DIRT);

				chunk.pBlocks[x + z * m_ChunkSize + y * m_ChunkSizeSqr] = pBlock;
			}

			float bigVegitationNoise{ m_BigVegitationPerlin.GetNoise(static_cast<float>(worldPosX) / m_ChunkSize, static_cast<float>(worldPosZ) / m_ChunkSize) };

			constexpr float vegitationSpawnChance{ 0.65f };
			if (bigVegitationNoise > vegitationSpawnChance)
			{
				if (chunk.pBlocks[x + z * m_ChunkSize + surfaceY * m_ChunkSizeSqr] == biome.bigVegitation->pSpawnOnBlock)
				{
					m_StructuresToSpawn.push_back(std::make_pair(biome.bigVegitation, XMINT3{ worldPosX,surfaceY + 1,worldPosZ }));
				}
			}
		}
	}

	chunks.push_back(chunk);
	m_WaterChunks.push_back(waterChunk);
}

void WorldGenerator::SpawnStructure(std::vector<Chunk>& chunks, const Structure* structure, const XMINT3& position)
{
	for (const StructureBlock& b : structure->blocks)
	{
		const XMINT3 bPos
		{
			position.x + b.position.x,
			position.y + b.position.y,
			position.z + b.position.z
		};

		const XMINT2 chunkPos
		{
			bPos.x < 0 ? (static_cast<int>(bPos.x) + 1) / m_ChunkSize - 1 : static_cast<int>(bPos.x) / m_ChunkSize,
			bPos.z < 0 ? (static_cast<int>(bPos.z) + 1) / m_ChunkSize - 1 : static_cast<int>(bPos.z) / m_ChunkSize
		};

		auto it{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
			}) };
		if (it == chunks.end()) return;

		const XMINT3 lookUpPos{ static_cast<int>(bPos.x) - chunkPos.x * m_ChunkSize, static_cast<int>(bPos.y), static_cast<int>(bPos.z) - chunkPos.y * m_ChunkSize };

		if (lookUpPos.x < 0 || lookUpPos.x >= m_ChunkSize
			|| lookUpPos.z < 0 || lookUpPos.z >= m_ChunkSize
			|| lookUpPos.y < 0 || lookUpPos.y >= m_WorldHeight) return;

		const int blockIdx{ lookUpPos.x + lookUpPos.z * m_ChunkSize + lookUpPos.y * m_ChunkSize * m_ChunkSize };

		it->pBlocks[blockIdx] = b.pBlock;
	}
}

Block* WorldGenerator::GetBlock(const XMINT3& position, float worldHeight, int surfaceY, float beachSize, const Biome& biome) const
{
	if (position.y == 0) return BlockManager::Get()->GetBlock(BlockType::BEDROCK);

	if (position.y <= m_SeaLevel && position.y > worldHeight) return m_pWaterBlock.get();

	if (position.y <= m_SeaLevel + beachSize) return biome.beach.pBlock;

	if (position.y == surfaceY) return biome.topLayer;

	Block* pRestLayer{};
	int curY{ surfaceY - 1 };

	for (const BlockLayer& layer : biome.layers)
	{
		if (layer.size == -1)
		{
			pRestLayer = layer.pBlock;
			break;
		}

		if (position.y > curY - layer.size) return layer.pBlock;

		curY -= layer.size;
	}

	return pRestLayer;
}
