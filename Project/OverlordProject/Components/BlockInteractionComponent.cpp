#include "stdafx.h"
#include "BlockInteractionComponent.h"

#include "WorldComponent.h"
#include "Components/WireframeRenderer.h"

#include "Managers/InputManager.h"

BlockInteractionComponent::BlockInteractionComponent(WorldComponent* pWorld, WireframeRenderer* pSelection)
	: m_pWorld{ pWorld }
	, m_pSelection{ pSelection }
{
}

void BlockInteractionComponent::Update(const SceneContext& sceneContext)
{
	if (InputManager::IsMouseButton(InputState::pressed, 1))
	{
		if (!m_pSelection->IsVisible()) return;

		m_pWorld->DestroyBlock(m_pSelection->GetTransform()->GetWorldPosition(), sceneContext);
	}
}
