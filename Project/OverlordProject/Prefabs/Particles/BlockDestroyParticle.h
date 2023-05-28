#pragma once
#include <Misc/World/WorldData.h>

class BlockDestroyParticle final : public GameObject
{
public:
	BlockDestroyParticle(BlockType block);

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext& sceneContext) override;

private:
	BlockType m_Block{};

	float m_MaxTime{1.0f};
	float m_CurTime{};
};

