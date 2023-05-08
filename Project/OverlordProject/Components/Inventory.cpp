#include "stdafx.h"
#include "Inventory.h"

void Inventory::Add(BlockType pBlock)
{
	int& nrItems{ m_Hotbar[pBlock] };

	++nrItems;
}
