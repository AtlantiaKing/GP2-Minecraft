#include "stdafx.h"
#include "BlockManager.h"
#include "Misc/Json/JsonReader.h"

BlockManager::BlockManager()
{
	JsonReader json{};
	m_pBlocks = json.ReadBlocks();
}

Block* BlockManager::GetBlock(const std::string& identifier)
{
	const auto it{ m_pBlocks.find(identifier) };

	if (it != m_pBlocks.end())
	{
		return it->second;
	}

	return nullptr;
}
