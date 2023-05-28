#include "stdafx.h"
#include "BlockBreakParticle.h"

void BlockBreakParticle::Initialize(const SceneContext&)
{
	//Particle System
	ParticleEmitterSettings settings{};
	settings.minVelocity = { -1.0f, 0.2f, -1.0f };
	settings.maxVelocity = { 1.0f, 0.2f, 1.0f };
	settings.minSize = 0.1f;
	settings.maxSize = 0.1f;
	settings.minEnergy = 1.f;
	settings.maxEnergy = 2.f;
	settings.minScale = 1.0f;
	settings.maxScale = 1.0f;
	settings.minEmitterRadius = 0.0f;
	settings.maxEmitterRadius = 0.1f;
	settings.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	settings.spriteSize = { 1.0f / 16.0f, 1.0f / 16.0f };
	settings.spritePivot.x = static_cast<int>(m_Block) % 16 / 16.0f;
	settings.spritePivot.y = (static_cast<int>(m_Block) / 16) / 16.0f;
	settings.gravity = -9.81f;
	settings.active = false;

	constexpr float distanceFromCenter{ 0.25f };

	for (int x{}; x <= 1; ++x)
	{
		for (int y{}; y <= 1; ++y)
		{
			GameObject* pParticleChild{ AddChild(new GameObject{}) };

			pParticleChild->GetTransform()->Translate(-distanceFromCenter + distanceFromCenter * x * 2, -distanceFromCenter + distanceFromCenter * y * 2, 0.5f);

			m_pEmitters.push_back(pParticleChild->AddComponent(new ParticleEmitterComponent(L"Textures/BlockParticles.dds", settings, 10)));
		}
	}
}

void BlockBreakParticle::SetBlock(BlockType block)
{
	m_Block = block;

	for (auto pEmitter : m_pEmitters)
	{
		pEmitter->GetSettings().spritePivot.x = static_cast<int>(block) % 16 / 16.0f;
		pEmitter->GetSettings().spritePivot.y = (static_cast<int>(block) / 16) / 16.0f;
	}
}

void BlockBreakParticle::SetActive(bool active)
{
	for (auto pEmitter : m_pEmitters)
	{
		pEmitter->GetSettings().active = active;
	}
}
