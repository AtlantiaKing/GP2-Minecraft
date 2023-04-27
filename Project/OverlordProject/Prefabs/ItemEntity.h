#pragma once

#include "Scenegraph/GameObject.h"
#include "Misc/World/WorldData.h"

class ItemEntity final : public GameObject
{
public:
	ItemEntity(BlockType block, const XMFLOAT3& blockPos);
	~ItemEntity() override = default;

	ItemEntity(const ItemEntity& other) = delete;
	ItemEntity(ItemEntity&& other) noexcept = delete;
	ItemEntity& operator=(const ItemEntity& other) = delete;
	ItemEntity& operator=(ItemEntity&& other) noexcept = delete;

	virtual void Initialize(const SceneContext& sceneContext) override;
private:
	XMFLOAT3 m_Position{};
	BlockType m_Block{};
};

