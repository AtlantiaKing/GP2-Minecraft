#include "stdafx.h"
#include "EntityDeathParticle.h"

void EntityDeathParticle::Initialize(const SceneContext&)
{
	//Particle System
	ParticleEmitterSettings settings{};
	settings.minVelocity = { -0.25f, 0.05f, -0.25f };
	settings.maxVelocity = { 0.25f, 0.4f, 0.25f };
	settings.minSize = 0.3f;
	settings.maxSize = 0.3f;
	settings.minEnergy = m_MaxTime;
	settings.maxEnergy = m_MaxTime;
	settings.minScale = 1.0f;
	settings.maxScale = 1.0f;
	settings.minEmitterRadius = 0.0f;
	settings.maxEmitterRadius = 0.5f;
	settings.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	settings.spriteSize = { 1.0f / m_NrSprites, 1.0f };
	settings.burst = true;

	m_pEmitter = AddComponent(new ParticleEmitterComponent(L"Textures/DeathParticle.dds", settings, 20));
}

void EntityDeathParticle::Update(const SceneContext& sceneContext)
{
	const float elapsedTime{ sceneContext.pGameTime->GetElapsed() };
	m_CurSpriteTime += elapsedTime;
	m_CurTotalTime += elapsedTime;

	if (m_CurSpriteTime > m_MaxTime / m_NrSprites)
	{
		m_CurSpriteTime = 0.0f;
		++m_CurSprite;
		m_pEmitter->GetSettings().spritePivot.x = m_CurSprite * m_pEmitter->GetSettings().spriteSize.x;
	}

	if (m_CurTotalTime > m_MaxTime) GetScene()->RemoveChild(this, true);
}
