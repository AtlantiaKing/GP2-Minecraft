#include "stdafx.h"
#include "BlockDestroyParticle.h"

BlockDestroyParticle::BlockDestroyParticle(BlockType block)
	: m_Block{ block }
{
}

void BlockDestroyParticle::Initialize(const SceneContext&)
{
	//Particle System
	ParticleEmitterSettings settings{};
	settings.minVelocity = { -2.0f, 1.0f, -2.0f };
	settings.maxVelocity = { 2.0f, 2.0f, 2.0f };
	settings.minSize = 0.1f;
	settings.maxSize = 0.1f;
	settings.minEnergy = 1.f;
	settings.maxEnergy = 2.f;
	settings.minScale = 1.0f;
	settings.maxScale = 1.0f;
	settings.minEmitterRadius = 0.0f;
	settings.maxEmitterRadius = 0.5f;
	settings.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	settings.spriteSize = { 1.0f / 16.0f, 1.0f / 16.0f };
	settings.spritePivot.x = static_cast<int>(m_Block) % 16 / 16.0f;
	settings.spritePivot.y = (static_cast<int>(m_Block) / 16) / 16.0f;
	settings.gravity = -9.81f;
	settings.burst = true;

	AddComponent(new ParticleEmitterComponent(L"Textures/BlockParticles.dds", settings, 20));
}

void BlockDestroyParticle::Update(const SceneContext& sceneContext)
{
	m_CurTime += sceneContext.pGameTime->GetElapsed();

	if (m_CurTime > m_MaxTime) GetScene()->RemoveChild(this, true);
}