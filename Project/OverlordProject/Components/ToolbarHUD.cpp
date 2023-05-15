#include "stdafx.h"
#include "ToolbarHUD.h"

#include "Inventory.h"
#include "Managers/BlockManager.h"
#include "ItemCounter.h"

ToolbarHUD::ToolbarHUD(Inventory* pInventory, const XMFLOAT2& screenSize)
	: m_pInventory{ pInventory }
	, m_ScreenSize{ screenSize }
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
		GameObject* pItemObj{ pChildren[i] };

		SpriteComponent* pItemRenderer{ pItemObj->GetComponent<SpriteComponent>() };

		pItemRenderer->SetTexture(L"Textures/InventoryIcons/air.dds");

		GameObject* pAmountObj{ pItemObj->GetChild<GameObject>() };
		pAmountObj->GetComponent<ItemCounter>()->SetAmount(0);
	}

	const int nrItems{ m_pInventory->GetMaxItems() };
	const auto& hotbarSize{ GetGameObject()->GetComponent<SpriteComponent>()->GetSize() };
	constexpr float hotbarMargin{ 4.0f };
	const float itemSize{ (hotbarSize.x - hotbarMargin * 2.0f) / nrItems };

	m_pSelection->GetTransform()->Translate(m_ScreenSize.x / 2.0f - itemSize * nrItems / 2.0f + itemSize / 2.0f + m_pInventory->GetSlot() * itemSize, m_ScreenSize.y, 0.0f);
}

void ToolbarHUD::OnSubjectDestroy()
{
	m_pInventory = nullptr;
}

void ToolbarHUD::SetSelection(GameObject* pSelection)
{
	m_pSelection = pSelection;
}
