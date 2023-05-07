#include "stdafx.h"
#include "WorldGenerator.h"

#include "Misc/World/WorldData.h"
#include "Managers/BlockManager.h"

WorldGenerator::WorldGenerator()
	: m_HeightPerlin{ 4, 5 }
	, m_UnderSeaPerlin{ 5, 25 }
	, m_BeachPerlin{ 2, 1 }
	, m_VegitationPerlin{ 5, 0.1f }
{
	m_IsBlockPredicate = [&](const std::vector<Chunk>& chunks, const XMINT3& position) -> bool
	{
		Block* const* pBlock{ GetBlockInChunk(position.x, position.y, position.z, chunks) };

		if (!pBlock) return false;

		return *pBlock;
	};
	m_CanRenderPredicate = [&](const std::vector<Chunk>& chunks, const XMINT3& neighbourPos, BlockType curBlock) -> bool
	{
		Block* const* pNeighbourBlock{ GetBlockInChunk(neighbourPos.x, neighbourPos.y, neighbourPos.z, chunks) };

		if (!pNeighbourBlock || !*pNeighbourBlock) return true;

		if ((*pNeighbourBlock)->mesh == BlockMesh::CROSS) return true;

		if (curBlock != BlockType::WATER && (*pNeighbourBlock)->transparent) return true;

		return false;
	};

	m_pWaterBlock = std::make_unique<Block>(BlockType::WATER, BlockMesh::CUBE, nullptr, -1.0f, true);

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
void WorldGenerator::RemoveBlock(std::vector<Chunk>& chunks, const XMFLOAT3& position)
{
	Block** pBlock{ GetBlockInChunk(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.z), chunks) };
	if (!pBlock || !*pBlock) return;

	*pBlock = nullptr;

	Block** pBlockUp{ GetBlockInChunk(static_cast<int>(position.x), static_cast<int>(position.y) + 1, static_cast<int>(position.z), chunks) };
	if (pBlockUp && (*pBlockUp) && (*pBlockUp)->mesh == BlockMesh::CROSS) *pBlockUp = nullptr;

	ReloadChunks(chunks, static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.z));
}

void WorldGenerator::PlaceBlock(std::vector<Chunk>& chunks, const XMFLOAT3& position, BlockType block)
{
	Block** pBlock{ GetBlockInChunk(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.z), chunks) };

	if (!pBlock) return;

	*pBlock = BlockManager::Get()->GetBlock(block);

	Block** pBlockDown{ GetBlockInChunk(static_cast<int>(position.x), static_cast<int>(position.y) - 1, static_cast<int>(position.z), chunks) };
	if (pBlockDown && (*pBlockDown) && (*pBlockDown)->type == BlockType::GRASS_BLOCK) *pBlockDown = BlockManager::Get()->GetBlock(BlockType::DIRT);

	ReloadChunks(chunks, static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.z));
}

void WorldGenerator::ReloadChunks(std::vector<Chunk>& chunks, int changedX, int changedY, int changedZ)
{
	Chunk* pChunk{ GetChunkAt(changedX, changedZ, chunks) };
	Chunk* pWaterChunk{ GetChunkAt(changedX, changedZ, m_WaterChunks) };
	const XMINT3 lookUpPos{ static_cast<int>(changedX) - pChunk->position.x * m_ChunkSize, static_cast<int>(changedY), static_cast<int>(changedZ) - pChunk->position.y * m_ChunkSize };

	std::vector<std::vector<Chunk>*> predicateChunks{};
	predicateChunks.push_back(&chunks);

	std::vector<std::vector<Chunk>*> predicateWaterChunks{};
	predicateWaterChunks.push_back(&chunks);
	predicateWaterChunks.push_back(&m_WaterChunks);

	CreateVertices(*pChunk, predicateChunks);
	CreateVertices(*pWaterChunk, predicateWaterChunks);

	if (lookUpPos.x == 0 || lookUpPos.x == m_ChunkSize - 1)
	{
		const int otherChunkX{ lookUpPos.x == 0 ? pChunk->position.x - 1 : pChunk->position.x + 1 };

		auto neighbourIt{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == otherChunkX && chunk.position.y == pChunk->position.y;
			}) };
		auto neighbourWaterIt{ std::find_if(begin(m_WaterChunks), end(m_WaterChunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == otherChunkX && chunk.position.y == pChunk->position.y;
			}) };

		if (neighbourIt != chunks.end())
			CreateVertices(*neighbourIt, predicateChunks);
		if (neighbourWaterIt != m_WaterChunks.end())
			CreateVertices(*neighbourIt, predicateWaterChunks);
	}

	if (lookUpPos.z == 0 || lookUpPos.z == m_ChunkSize - 1)
	{
		const int otherChunkY{ lookUpPos.z == 0 ? pChunk->position.y - 1 : pChunk->position.y + 1 };

		auto neighbourIt{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == pChunk->position.x && chunk.position.y == otherChunkY;
			}) };
		auto neighbourWaterIt{ std::find_if(begin(m_WaterChunks), end(m_WaterChunks), [&](const Chunk& chunk)
			{
				return chunk.position.x == pChunk->position.x && chunk.position.y == otherChunkY;
			}) };

		if (neighbourIt != chunks.end())
			CreateVertices(*neighbourIt, predicateChunks);
		if (neighbourWaterIt != m_WaterChunks.end())
			CreateVertices(*neighbourIt, predicateWaterChunks);
	}
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
					removeBlocks.emplace_back(position);

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
					newBlocks.emplace_back(neighbourPosition);

					// Keep track that the environment should be reset
					changedEnvironment = true;
				}
			}
		}
	}

	// If the environment has been changed
	if (changedEnvironment)
	{
		std::vector<Chunk*> chunksThatNeedUpdate{};

		// Add the new water blocks to the water object
		for (const XMINT3& block : newBlocks)
		{
			Block** pWaterBlock{ GetBlockInChunk(static_cast<int>(block.x),static_cast<int>(block.y),static_cast<int>(block.z), m_WaterChunks) };

			*pWaterBlock = m_pWaterBlock.get();

			Chunk* pChunk{ GetChunkAt(static_cast<int>(block.x),static_cast<int>(block.z), m_WaterChunks) };

			if (std::find_if(begin(chunksThatNeedUpdate), end(chunksThatNeedUpdate), [pChunk](Chunk* pUpdateChunk) { return pUpdateChunk == pChunk; }) == end(chunksThatNeedUpdate))
				chunksThatNeedUpdate.push_back(pChunk);
		}

		// remove the blocks from the water object
		for (const XMINT3& block : removeBlocks)
		{
			Block** pWaterBlock{ GetBlockInChunk(static_cast<int>(block.x),static_cast<int>(block.y),static_cast<int>(block.z), m_WaterChunks) };

			*pWaterBlock = nullptr;

			Chunk* pChunk{ GetChunkAt(static_cast<int>(block.x),static_cast<int>(block.z), m_WaterChunks) };

			if (std::find_if(begin(chunksThatNeedUpdate), end(chunksThatNeedUpdate), [pChunk](Chunk* pUpdateChunk) { return pUpdateChunk == pChunk; }) == end(chunksThatNeedUpdate))
				chunksThatNeedUpdate.push_back(pChunk);
		}

		// Reload the water vertices
		std::vector<std::vector<Chunk>*> predicateChunks{};
		predicateChunks.push_back(&chunks);
		predicateChunks.push_back(&m_WaterChunks);
		for (Chunk* pChunk : chunksThatNeedUpdate)
		{
			CreateVertices(*pChunk, predicateChunks);
		}
	}

	// Return whether the environment has been changed or not
	return changedEnvironment;
}

void WorldGenerator::CreateVertices(Chunk& chunk, const std::vector<std::vector<Chunk>*>& predicateChunks)
{
	chunk.verticesChanged = true;
	chunk.needColliderChange = true;

	std::vector<VertexPosNormTexTransparency> vertices{};
	for (int x{}; x < m_ChunkSize; ++x)
	{
		for (int z{}; z < m_ChunkSize; ++z)
		{
			for (int y{ m_WorldHeight - 1 }; y >= 0; --y)
			{
				Block* pBlock{ chunk.pBlocks[x + z * m_ChunkSize + y * m_ChunkSize * m_ChunkSize] };

				if (!pBlock) continue;

				switch (pBlock->mesh)
				{
				case BlockMesh::CUBE:
					CreateVerticesCube(chunk, x, y, z, predicateChunks, pBlock, vertices);
					break;
				case BlockMesh::CROSS:
					CreateVerticesCross(chunk, x, y, z, pBlock, vertices);
				}
			}
		}
	}
	chunk.vertices = std::move(vertices);

	chunk.loadedWater = true;
}

void WorldGenerator::CreateVerticesCube(Chunk& chunk, int x, int y, int z, const std::vector<std::vector<Chunk>*>& predicateChunks, Block* pBlock, std::vector<VertexPosNormTexTransparency>& vertices)
{
	const auto& cubeVertices{ BlockManager::Get()->GetVertices("cube") };

	const XMINT3 position{ chunk.position.x * m_ChunkSize + x, y, chunk.position.y * m_ChunkSize + z };

	for (unsigned int i{}; i <= static_cast<unsigned int>(FaceDirection::BOTTOM); ++i)
	{
		const XMINT3& neightbourDirection{ m_NeighbouringBlocks[i] };

		const XMVECTOR positionVector = XMLoadSInt3(&position);
		const XMVECTOR neighbourDirection = XMLoadSInt3(&neightbourDirection);
		XMINT3 neighbourPosition{};
		const XMVECTOR neighbourPosVector = XMVectorAdd(positionVector, neighbourDirection);
		XMStoreSInt3(&neighbourPosition, neighbourPosVector);

		bool canRender{ true };
		for (const auto& chunks : predicateChunks)
		{
			if (!m_CanRenderPredicate(*chunks, neighbourPosition, pBlock->type))
			{
				canRender = false;
				break;
			}
		}
		if (!canRender) continue;

		constexpr int faceIndices[6]{ 0,1,2,3,2,1 };
		for (int vIdx : faceIndices)
		{
			VertexPosNormTexTransparency v{ cubeVertices[i * 4 + vIdx] };

			if (pBlock->type == BlockType::WATER && !m_IsBlockPredicate(m_WaterChunks, { x,y + 1,z }))
			{
				constexpr float waterOffset{ 0.125 };
				if (v.Position.y > 0.0f) v.Position.y -= waterOffset;
			}

			XMVECTOR pos{ XMLoadFloat3(&v.Position) };
			pos += XMVECTOR{ static_cast<float>(x),static_cast<float>(y),static_cast<float>(z) }
			+ XMVECTOR{ static_cast<float>(chunk.position.x * m_ChunkSize), 0.0f, static_cast<float>(chunk.position.y * m_ChunkSize) };
			XMStoreFloat3(&v.Position, pos);

			v.TexCoord = m_TileMap.GetUV(m_TileMap.GetFaceType(pBlock->type, static_cast<FaceDirection>(i)), v.TexCoord);

			v.Transparent = pBlock->transparent;

			vertices.emplace_back(v);
		}
	}
}

void WorldGenerator::CreateVerticesCross(Chunk& chunk, int x, int y, int z, Block* pBlock, std::vector<VertexPosNormTexTransparency>& vertices)
{
	const auto& crossVertices{ BlockManager::Get()->GetVertices("cross") };

	const XMINT3 position{ chunk.position.x * m_ChunkSize + x, y, chunk.position.y * m_ChunkSize + z };

	for (int faceIdx{}; faceIdx < static_cast<int>(crossVertices.size()) / 4; ++faceIdx)
	{
		constexpr int faceIndices[6]{ 0,1,2,3,2,1 };
		for (int vIdx : faceIndices)
		{
			VertexPosNormTexTransparency vertexCopy{ crossVertices[faceIdx * 4 + vIdx] };

			XMVECTOR pos{ XMLoadFloat3(&vertexCopy.Position) };
			pos += XMVECTOR{ static_cast<float>(position.x),static_cast<float>(position.y),static_cast<float>(position.z) };
			XMStoreFloat3(&vertexCopy.Position, pos);

			vertexCopy.TexCoord = m_TileMap.GetUV(m_TileMap.GetFaceType(pBlock->type, FaceDirection::FORWARD), vertexCopy.TexCoord);

			vertexCopy.Transparent = pBlock->transparent;

			vertices.emplace_back(vertexCopy);
		}
	}
}

Block* WorldGenerator::GetBlockAt(int x, int y, int z, const std::vector<Chunk>& chunks) const
{
	Block* const* pBlock{ GetBlockInChunk(x,y,z,chunks) };

	if (!pBlock) return nullptr;

	return *pBlock;
}

std::vector<XMFLOAT3> WorldGenerator::GetPositions(const Chunk& chunk) const
{
	std::vector<XMFLOAT3> vertices{};
	vertices.reserve(chunk.vertices.size());

	for (const VertexPosNormTexTransparency& v : chunk.vertices)
	{
		if (v.Transparent) continue;

		vertices.emplace_back(v.Position);
	}

	return vertices;
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

	std::vector<std::vector<Chunk>*> predicateChunks{};
	predicateChunks.push_back(&chunks);

	for (Chunk& chunk : chunks)
	{
		CreateVertices(chunk, predicateChunks);
	}

	predicateChunks.push_back(&m_WaterChunks);

	for (Chunk& chunk : m_WaterChunks)
	{
		CreateVertices(chunk, predicateChunks);
	}
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

				if (pBlock->type == BlockType::DIRT || pBlock->type == BlockType::GRASS_BLOCK) hasDirt = true;
				if (pBlock->type == BlockType::SAND && hasDirt) pBlock = BlockManager::Get()->GetBlock(BlockType::DIRT);

				chunk.pBlocks[x + z * m_ChunkSize + y * m_ChunkSizeSqr] = pBlock;
			}

			float vegitationNoise{ m_VegitationPerlin.GetNoise(static_cast<float>(worldPosX) / m_ChunkSize, static_cast<float>(worldPosZ) / m_ChunkSize) };

			constexpr float bigVegitationSpawnChance{ 0.7f };	
			constexpr float smallVegitationSpawnChance{ 0.5f };
			if (vegitationNoise > bigVegitationSpawnChance)
			{
				if (biome.bigVegitation != nullptr && chunk.pBlocks[x + z * m_ChunkSize + surfaceY * m_ChunkSizeSqr] == biome.bigVegitation->pSpawnOnBlock)
				{
					m_StructuresToSpawn.emplace_back(std::make_pair(biome.bigVegitation, XMINT3{ worldPosX,surfaceY + 1,worldPosZ }));
				}
			}
			else if (vegitationNoise < smallVegitationSpawnChance)
			{
				if (biome.smallVegitation != nullptr && chunk.pBlocks[x + z * m_ChunkSize + surfaceY * m_ChunkSizeSqr] == biome.bigVegitation->pSpawnOnBlock)
				{
					m_StructuresToSpawn.emplace_back(std::make_pair(biome.smallVegitation, XMINT3{ worldPosX,surfaceY + 1,worldPosZ }));
				}
			}
		}
	}

	chunks.emplace_back(chunk);
	m_WaterChunks.emplace_back(waterChunk);
}

Block* WorldGenerator::GetBlock(const XMINT3& position, float worldHeight, int surfaceY, float beachSize, const Biome& biome) const
{
	if (position.y == 0) return BlockManager::Get()->GetBlock(BlockType::BEDROCK);

	if (position.y <= m_SeaLevel && position.y > worldHeight) return m_pWaterBlock.get();

	if (surfaceY <= m_SeaLevel + beachSize && position.y > m_SeaLevel - beachSize) return biome.beach.pBlock;

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

		Block** pBlock{ GetBlockInChunk(bPos.x, bPos.y, bPos.z, chunks) };

		if (!pBlock) return;

		*pBlock = b.pBlock;

		if (b.pBlock->mesh == BlockMesh::CUBE)
		{
			Chunk* pChunk{ GetChunkAt(static_cast<int>(position.x), static_cast<int>(position.z), chunks) };
			const XMINT3 lookUpPos{ static_cast<int>(position.x) - pChunk->position.x * m_ChunkSize, static_cast<int>(position.y), static_cast<int>(position.z) - pChunk->position.y * m_ChunkSize };

			const int blockUnderIdx{ lookUpPos.x + lookUpPos.z * m_ChunkSize + (lookUpPos.y - 1) * m_ChunkSize * m_ChunkSize };
			if (pChunk->pBlocks[blockUnderIdx] && pChunk->pBlocks[blockUnderIdx]->type == BlockType::GRASS_BLOCK) pChunk->pBlocks[blockUnderIdx] = BlockManager::Get()->GetBlock(BlockType::DIRT);
		}
	}
}

Block** WorldGenerator::GetBlockInChunk(int x, int y, int z, std::vector<Chunk>& chunks) const
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

	return it->pBlocks.data() + blockIdx;
}

Block* const* WorldGenerator::GetBlockInChunk(int x, int y, int z, const std::vector<Chunk>& chunks) const
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

	return it->pBlocks.data() + blockIdx;
}

Chunk* WorldGenerator::GetChunkAt(int x, int z, std::vector<Chunk>& chunks) const
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

	return it._Unwrapped();
}