#pragma once

#include "Components/BaseComponent.h"

class EntityInteractionComponent final : public BaseComponent
{
public:
	EntityInteractionComponent(PxScene* pxScene);
	virtual ~EntityInteractionComponent() = default;

	EntityInteractionComponent(const EntityInteractionComponent& other) = delete;
	EntityInteractionComponent(EntityInteractionComponent&& other) noexcept = delete;
	EntityInteractionComponent& operator=(const EntityInteractionComponent& other) = delete;
	EntityInteractionComponent& operator=(EntityInteractionComponent&& other) noexcept = delete;

protected:
	virtual void Initialize(const SceneContext& /*sceneContext*/) override {};
	virtual void Update(const SceneContext& sceneContext) override;

private:
	PxScene* m_PxScene{};
};

