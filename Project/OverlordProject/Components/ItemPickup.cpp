#include "stdafx.h"
#include "ItemPickup.h"

#include "Misc/World/WorldData.h"

#include "Components/Inventory.h"

ItemPickup::ItemPickup(BlockType block)
	: m_Block{ block }
{
}

void ItemPickup::Remove()
{
	m_Remove = true;
}

void ItemPickup::Initialize(const SceneContext&)
{
	GetGameObject()->SetOnTriggerCallBack(OnTriggerEvent);
}

void ItemPickup::Update(const SceneContext&)
{
	if(m_Remove) GetScene()->RemoveChild(GetGameObject(), true);
}

void ItemPickup::OnTriggerEvent(GameObject* pTriggerObject, GameObject* pOtherObject, PxTriggerAction action)
{
	if (action != PxTriggerAction::ENTER) return;

	Inventory* pInventory{ pOtherObject->GetComponent<Inventory>() };
	if (!pInventory) return;

	ItemPickup* pItem{ pTriggerObject->GetComponent<ItemPickup>() };
	BlockType pBlock{ pItem->GetBlock() };

	pInventory->Add(pBlock);

	pItem->Remove();
}
