#include "stdafx.h"
#include "BlockInteractionComponent.h"

#include "WorldComponent.h"
#include "Components/WireframeRenderer.h"

#include "Managers/InputManager.h"

BlockInteractionComponent::BlockInteractionComponent(PxScene* pxScene, WorldComponent* pWorld, WireframeRenderer* pSelection)
	: m_pWorld{ pWorld }
	, m_pSelection{ pSelection }
	, m_PxScene{ pxScene }
{
}

void BlockInteractionComponent::Update(const SceneContext& sceneContext)
{
	TransformComponent* pCamera{ sceneContext.pCamera->GetTransform() };
	constexpr float playerBlockRadius{ 5.0f };

	const XMFLOAT3 cameraPosition{ pCamera->GetWorldPosition() };
	const PxVec3 raycastOrigin{ cameraPosition.x, cameraPosition.y, cameraPosition.z };
	const XMFLOAT3 cameraForward{ pCamera->GetForward() };
	const PxVec3 raycastDirection{ cameraForward.x, cameraForward.y, cameraForward.z };

	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);

	PxRaycastBuffer hit;
	if (m_PxScene->raycast(raycastOrigin, raycastDirection, playerBlockRadius, hit, PxHitFlag::eDEFAULT, filter))
	{
		if (!hit.hasBlock)
		{
			m_pSelection->SetVisibility(false);
			return;
		}

		m_pSelection->SetVisibility(true);

		const PxVec3 hitPos{ hit.block.position + raycastDirection * 0.01f };
		const XMFLOAT3 blockPos
		{
			(hitPos.x + 0.5f) > 0.0f ? floor(hitPos.x + 0.5f) : -(floor(abs(hitPos.x + 0.5f)) + 1),
			(hitPos.y + 0.5f) > 0.0f ? floor(hitPos.y + 0.5f) : -(floor(abs(hitPos.y + 0.5f)) + 1),
			(hitPos.z + 0.5f) > 0.0f ? floor(hitPos.z + 0.5f) : -(floor(abs(hitPos.z + 0.5f)) + 1)
		};
		m_pSelection->GetTransform()->Translate(blockPos);


		if (InputManager::IsMouseButton(InputState::pressed, 1))
		{
			m_pWorld->DestroyBlock(m_pSelection->GetTransform()->GetWorldPosition());
		}
	}
	else
	{
		m_pSelection->SetVisibility(false);
	}
}
