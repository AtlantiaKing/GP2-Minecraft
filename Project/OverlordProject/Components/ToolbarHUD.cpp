#include "stdafx.h"
#include "ToolbarHUD.h"

#include "Inventory.h"
#include "Managers/BlockManager.h"
#include "ItemCounter.h"

ToolbarHUD::ToolbarHUD(Inventory* pInventory)
	: m_pInventory{ pInventory }
{
	pInventory->OnInventoryChange().AddListener(this);
}

ToolbarHUD::~ToolbarHUD()
{
	if(m_pInventory) m_pInventory->OnInventoryChange().RemoveListener(this);
}

void ToolbarHUD::Notify(const std::unordered_map<BlockType, int>& inventory)
{
	int curSlot{};

	const auto& pChildren{ m_pGameObject->GetChildren<GameObject>() };

	for (const auto& inventoryPair : inventory)
	{
		if (pChildren.size() == curSlot) return;

		GameObject* pItemObj{ pChildren[curSlot] };
		SpriteComponent* pItemRenderer{ pChildren[curSlot]->GetComponent<SpriteComponent>() };

		std::wstringstream filePath{};
		filePath << L"Textures/InventoryIcons/" << StringUtil::utf8_decode(BlockManager::Get()->GetBlockName(inventoryPair.first)) << ".dds";

		pItemRenderer->SetTexture(filePath.str());

		GameObject* pAmountObj{ pItemObj->GetChild<GameObject>() };
		pAmountObj->GetComponent<ItemCounter>()->SetAmount(inventoryPair.second);

		++curSlot;
	}

	for (int i{ curSlot }; i < pChildren.size(); ++i) 
	{
		SpriteComponent* pItemRenderer{ pChildren[i]->GetComponent<SpriteComponent>() };

		pItemRenderer->SetTexture(L"Textures/InventoryIcons/air.dds");
	}
}

void ToolbarHUD::OnSubjectDestroy()
{
	m_pInventory = nullptr;
}
