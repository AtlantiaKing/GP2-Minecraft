#pragma once

#include "Components/BaseComponent.h"

class BlockInteractionComponent;

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
	virtual void Initialize(const SceneContext& /*sceneContext*/) override;
	virtual void Update(const SceneContext& sceneContext) override;

private:
	void PlayHitSound();

	BlockInteractionComponent* m_pBlockInteraction{};
	PxScene* m_PxScene{};

	FMOD::Channel* m_pAudioChannel{ nullptr };
	std::vector<FMOD::Sound*> m_pSounds{};
	const int m_NrSounds{ 3 };
	const float m_HitSoundVolume{ 0.3f };

	const float m_PlayerHitRadius{ 5.0f };
};

