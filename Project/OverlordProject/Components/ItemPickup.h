#pragma once

#include "Components/BaseComponent.h"

#include "Misc/World/WorldData.h"

class ItemPickup final : public BaseComponent
{
public:
	ItemPickup(BlockType block);
	virtual ~ItemPickup() = default;

	ItemPickup(const ItemPickup& other) = delete;
	ItemPickup(ItemPickup&& other) noexcept = delete;
	ItemPickup& operator=(const ItemPickup& other) = delete;
	ItemPickup& operator=(ItemPickup&& other) noexcept = delete;

	BlockType GetBlock() { return m_Block; }

	void Remove();
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext& sceneContext) override;

private:
	static void OnTriggerEvent(GameObject* pGO, GameObject* pOther, PxTriggerAction action);

	bool m_Remove{};

	BlockType m_Block{};
};

