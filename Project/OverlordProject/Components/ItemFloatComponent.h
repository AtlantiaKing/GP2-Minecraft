#pragma once

#include "Components/BaseComponent.h"

class ItemFloatComponent final : public BaseComponent
{
public:
	ItemFloatComponent() = default;
	virtual ~ItemFloatComponent() = default;

	ItemFloatComponent(const ItemFloatComponent& other) = delete;
	ItemFloatComponent(ItemFloatComponent&& other) noexcept = delete;
	ItemFloatComponent& operator=(const ItemFloatComponent& other) = delete;
	ItemFloatComponent& operator=(ItemFloatComponent&& other) noexcept = delete;

protected:
	virtual void Initialize(const SceneContext& /*sceneContext*/) override {};
	virtual void Update(const SceneContext& sceneContext) override;

private:
	float curOffset{};
	float curRotation{};
};

