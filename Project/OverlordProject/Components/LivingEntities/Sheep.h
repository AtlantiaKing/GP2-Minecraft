#pragma once

#include "LivingEntity.h"

class Sheep : public LivingEntity
{
public:
	Sheep(const XMFLOAT3& hitboxDimensions);
	virtual ~Sheep();

	Sheep(const Sheep& other) = delete;
	Sheep(Sheep&& other) noexcept = delete;
	Sheep& operator=(const Sheep& other) = delete;
	Sheep& operator=(Sheep&& other) noexcept = delete;

	virtual void OnHit(int health) override;

protected:
	virtual void InitMaterials() override;
	virtual void UpdateState() override;
	virtual void UpdateMovement(float elapsedSec);

	virtual void EntityUpdate(const SceneContext& sceneContext) override;

private:
	void PlayBaaSound();

	enum class SheepState
	{
		Idle,
		Walking
	};

	float m_CurBaaTime{};
	float m_CurTimeBetweenBaas{};
	const XMFLOAT2 m_TimeBetweenBaasRange{ 2.0f, 7.0f };

	FMOD::Channel* m_pAudioChannel{ nullptr };
	FMOD::Channel* m_pWalkAudioChannel{ nullptr };

	std::vector<FMOD::Sound*> m_pSounds{};
	const int m_NrSounds{ 3 };
	FMOD::Sound* m_pWalkSound{};

	const float m_AudioDistance{ 16.0f };
};

