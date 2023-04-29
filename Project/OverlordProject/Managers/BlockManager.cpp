#include "stdafx.h"
#include "BlockManager.h"
#include "Misc/Json/JsonReader.h"

BlockManager::BlockManager()
{
	JsonReader json{};
	m_pBlocksByIdentifier = json.ReadBlocks();

	for (const auto& blockPair : m_pBlocksByIdentifier)
	{
		m_pBlocksByType[blockPair.second->type] = blockPair.second;
	}
}

BlockManager::~BlockManager()
{
	for (const auto& blockPair : m_pBlocksByIdentifier)
	{
		delete blockPair.second;
	}
}

Block* BlockManager::GetBlock(const std::string& identifier)
{
	const auto it{ m_pBlocksByIdentifier.find(identifier) };

	if (it != m_pBlocksByIdentifier.end())
	{
		return it->second;
	}

	return nullptr;
}

Block* BlockManager::GetBlock(BlockType type)
{
	const auto it{ m_pBlocksByType.find(type) };

	if (it != m_pBlocksByType.end())
	{
		return it->second;
	}

	return nullptr;
}
