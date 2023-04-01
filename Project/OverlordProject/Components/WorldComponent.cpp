#include "stdafx.h"
#include "WorldComponent.h"

WorldComponent::WorldComponent(const SceneContext& sceneContext)
{
	m_Renderer.LoadEffect(sceneContext);
}

void WorldComponent::Initialize(const SceneContext& sceneContext)
{
	m_Renderer.SetBuffer(m_Generator.LoadWorld(), sceneContext);
}

void WorldComponent::Draw(const SceneContext& sceneContext)
{
	m_Renderer.Draw(sceneContext);
}
