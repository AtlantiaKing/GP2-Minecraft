#pragma once

#include "Components/BaseComponent.h"

class ItemCounter final : public BaseComponent
{
public:
	ItemCounter() = default;
	virtual ~ItemCounter() = default;

	ItemCounter(const ItemCounter& other) = delete;
	ItemCounter(ItemCounter&& other) noexcept = delete;
	ItemCounter& operator=(const ItemCounter& other) = delete;
	ItemCounter& operator=(ItemCounter&& other) noexcept = delete;

	void SetAmount(int amount);
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext& sceneContext) override;

private:
	std::string m_Text{};
	SpriteFont* m_pFont{};
};

