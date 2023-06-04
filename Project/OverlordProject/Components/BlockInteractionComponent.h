#pragma once

#include "Components/BaseComponent.h"

class WorldComponent;
class WireframeRenderer;
class BlockBreakRenderer;
class BlockBreakParticle;

class BlockInteractionComponent : public BaseComponent
{
public:
	BlockInteractionComponent(PxScene* pxScene, WorldComponent* pWorld, WireframeRenderer* pSelection, BlockBreakRenderer* pBreakRenderer, BlockBreakParticle* pBlockBreakParticle);
	virtual ~BlockInteractionComponent() = default;

	BlockInteractionComponent(const BlockInteractionComponent& other) = delete;
	BlockInteractionComponent(BlockInteractionComponent&& other) noexcept = delete;
	BlockInteractionComponent& operator=(const BlockInteractionComponent& other) = delete;
	BlockInteractionComponent& operator=(BlockInteractionComponent&& other) noexcept = delete;

	bool ShouldPlayAnimation() const;
	bool IsBreakingBlock() const { return m_IsBreakingBlock; }

protected:
	virtual void Initialize(const SceneContext& /*sceneContext*/) override {};
	virtual void Update(const SceneContext& sceneContext) override;

private:
	bool HasChangedPosition(const XMFLOAT3& position);
	bool IsBlockInPlayer(XMINT3 hitBlock, const PxVec3& hitNormal) const;

	WorldComponent* m_pWorld{};
	WireframeRenderer* m_pSelection{};
	BlockBreakRenderer* m_pBreakRenderer{};
	BlockBreakParticle* m_pBlockBreakParticle{};

	PxScene* m_PxScene{};

	XMFLOAT3 m_PrevPosition{};

	bool m_IsBreakingBlock{};
	float m_BlockBreakProgress{};

	bool m_ShouldPlayAnimation{};
};

