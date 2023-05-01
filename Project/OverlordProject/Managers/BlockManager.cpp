#include "stdafx.h"
#include "BlockManager.h"
#include "Misc/FileReaders/JsonReader.h"
#include <Misc/FileReaders/ObjReader.h>

class BiomeNotFoundException {};
class MeshNotFoundException {};


BlockManager::BlockManager()
{
	JsonReader json{};
	m_pBlocksByIdentifier = json.ReadBlocks();

	for (const auto& blockPair : m_pBlocksByIdentifier)
	{
		m_pBlocksByType[blockPair.second->type] = blockPair.second;
	}

	m_StructuresByIdentifier = json.ReadStructures(m_pBlocksByIdentifier);

	m_BiomesByIdentifier = json.ReadBiomes(m_pBlocksByIdentifier, m_StructuresByIdentifier);


	const auto& meshNames{ json.ReadBlockTypes() };
	ObjReader obj{};
	for (const auto& meshName : meshNames)
	{
		m_VerticesByIdentifier[meshName] = obj.ReadVertices(meshName);
	}
}

BlockManager::~BlockManager()
{
	for (const auto& blockPair : m_pBlocksByIdentifier)
	{
		delete blockPair.second;
	}
}

Block* BlockManager::GetBlock(const std::string& identifier) const
{
	const auto it{ m_pBlocksByIdentifier.find(identifier) };

	if (it != m_pBlocksByIdentifier.end())
	{
		return it->second;
	}

	return nullptr;
}

Block* BlockManager::GetBlock(BlockType type) const
{
	const auto it{ m_pBlocksByType.find(type) };

	if (it != m_pBlocksByType.end())
	{
		return it->second;
	}

	return nullptr;
}

const std::vector<VertexPosNormTexTransparency>& BlockManager::GetVertices(const std::string& identifier) const
{
	const auto it{ m_VerticesByIdentifier.find(identifier) };

	if (it != m_VerticesByIdentifier.end())
	{
		return it->second;
	}

	throw MeshNotFoundException{};
}

const Biome& BlockManager::GetBiome(const std::string& identifier) const
{
	const auto it{ m_BiomesByIdentifier.find(identifier) };

	if (it != m_BiomesByIdentifier.end())
	{
		return it->second;
	}

	throw BiomeNotFoundException{};
}