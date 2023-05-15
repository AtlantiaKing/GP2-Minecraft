#include "stdafx.h"
#include "Inventory.h"

void Inventory::Add(BlockType pBlock)
{
	int& nrItems{ m_Hotbar[pBlock] };

	++nrItems;

	OnInventoryChange().Notify(m_Hotbar);
}

void Inventory::Remove(BlockType block)
{
	const auto& it{ m_Hotbar.find(block) };
	if (it == end(m_Hotbar)) return;

	--it->second;

	if (it->second <= 0) m_Hotbar.erase(it);

	OnInventoryChange().Notify(m_Hotbar);
}

bool Inventory::HasOfType(BlockType block) const
{
	const auto& it{ m_Hotbar.find(block) };
	if (it == end(m_Hotbar)) return false;
	return true;
}
