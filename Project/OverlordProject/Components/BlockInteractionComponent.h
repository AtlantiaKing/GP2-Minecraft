#pragma once

#include "Components/BaseComponent.h"

class WorldComponent;
class WireframeRenderer;

class BlockInteractionComponent : public BaseComponent
{
public:
	BlockInteractionComponent(PxScene* pxScene, WorldComponent* pWorld, WireframeRenderer* pSelection);
	virtual ~BlockInteractionComponent() = default;

	BlockInteractionComponent(const BlockInteractionComponent& other) = delete;
	BlockInteractionComponent(BlockInteractionComponent&& other) noexcept = delete;
	BlockInteractionComponent& operator=(const BlockInteractionComponent& other) = delete;
	BlockInteractionComponent& operator=(BlockInteractionComponent&& other) noexcept = delete;
protected:
	virtual void Initialize(const SceneContext& /*sceneContext*/) override {};
	virtual void Update(const SceneContext& sceneContext) override;
private:
	WorldComponent* m_pWorld{};
	WireframeRenderer* m_pSelection{};
	PxScene* m_PxScene{};

	bool m_IsBreakingBlock{};
	float m_BlockBreakProgress{};
};

