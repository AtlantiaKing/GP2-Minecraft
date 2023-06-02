#pragma once
class EntityDeathParticle final : public GameObject
{
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext& sceneContext) override;

private:
	const int m_NrSprites{ 8 };
	const float m_MaxTime{ 2.0f };

	ParticleEmitterComponent* m_pEmitter{};

	int m_CurSprite{};

	float m_CurSpriteTime{};
	float m_CurTotalTime{};
};

