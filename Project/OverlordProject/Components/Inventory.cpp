#include "stdafx.h"
#include "Inventory.h"

void Inventory::Add(BlockType pBlock)
{
	int& nrItems{ m_Hotbar[pBlock] };

	++nrItems;
}

void Inventory::Remove(BlockType block)
{
	const auto& it{ m_Hotbar.find(block) };
	if (it == end(m_Hotbar)) return;

	--it->second;

	if (it->second <= 0) m_Hotbar.erase(it);
}

bool Inventory::HasOfType(BlockType block) const
{
	const auto& it{ m_Hotbar.find(block) };
	if (it == end(m_Hotbar)) return false;
	return true;
}
