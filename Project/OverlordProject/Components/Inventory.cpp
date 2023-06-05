#include "stdafx.h"
#include "Inventory.h"

void Inventory::Update(const SceneContext& sceneContext)
{
	if (sceneContext.pInput->IsKeyboardKey(InputState::pressed, VK_RIGHT)
		|| sceneContext.pInput->IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_DPAD_RIGHT))
	{
		++m_CurSlot %= m_MaxItems;
		OnInventoryChange().Notify(m_Hotbar);
		return;
	}
	else if (sceneContext.pInput->IsKeyboardKey(InputState::pressed, VK_LEFT)
		|| sceneContext.pInput->IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_DPAD_LEFT))
	{
		--m_CurSlot;
		if (m_CurSlot < 0) m_CurSlot = m_MaxItems - 1;
		OnInventoryChange().Notify(m_Hotbar);
		return;
	}

	for (char c{ '0' }; c <= '9'; ++c)
	{
		if (!sceneContext.pInput->IsKeyboardKey(InputState::pressed, c)) continue;
			
		if(c == '0')
			m_CurSlot = 9;
		else
			m_CurSlot = static_cast<int>(c - '1');

		OnInventoryChange().Notify(m_Hotbar);

		break;
	}
}

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

bool Inventory::HasEquipedItem() const
{
	for (int i{}; i < m_Hotbar.size(); ++i)
	{
		if (i == m_CurSlot) return true;
	}

	return false;
}

BlockType Inventory::GetEquipedItem() const
{
	int slot{};
	for (const auto& itemPair : m_Hotbar)
	{
		if (slot == m_CurSlot) return itemPair.first;
		++slot;
	}
	return BlockType::DIRT;
}

int Inventory::GetMaxItems() const
{
	return m_MaxItems;
}

int Inventory::GetSlot() const
{
	return m_CurSlot;
}
