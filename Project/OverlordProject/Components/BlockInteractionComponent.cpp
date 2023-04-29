#include "stdafx.h"
#include "BlockInteractionComponent.h"

#include "WorldComponent.h"
#include "Rendering/WireframeRenderer.h"
#include "Rendering/BlockBreakRenderer.h"

#include "Managers/InputManager.h"

BlockInteractionComponent::BlockInteractionComponent(PxScene* pxScene, WorldComponent* pWorld, WireframeRenderer* pSelection, BlockBreakRenderer* pBreakRenderer)
	: m_pWorld{ pWorld }
	, m_pSelection{ pSelection }
	, m_PxScene{ pxScene }
	, m_pBreakRenderer{ pBreakRenderer }
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

		// Get current block
		Block* pBlock{ m_pWorld->GetBlockAt(static_cast<int>(blockPos.x), static_cast<int>(blockPos.y), static_cast<int>(blockPos.z)) };

		if (HasChangedPosition(blockPos)) m_IsBreakingBlock = false;

		if (InputManager::IsMouseButton(InputState::pressed, 2))
		{
			m_pWorld->PlaceBlock(XMFLOAT3{ hitPos.x, hitPos.y, hitPos.z }, blockPos, BlockType::DIRT);
			m_IsBreakingBlock = false;
		}
		
		if (m_IsBreakingBlock)
		{
			if (InputManager::IsMouseButton(InputState::down, 1))
			{
				if (pBlock)
				{
					// Increment breaking time
					m_BlockBreakProgress += sceneContext.pGameTime->GetElapsed();

					// Destroy the block if the progress has reached 100%
					if (m_BlockBreakProgress > pBlock->breakTime)
					{
						m_pWorld->DestroyBlock(blockPos);
					}
				}
			}
			else
			{
				m_IsBreakingBlock = false;
			}
		}
		else if (InputManager::IsMouseButton(InputState::down, 1))
		{
			m_IsBreakingBlock = true;
			m_BlockBreakProgress = 0.0f;
		}


		// Enable/Disable the breaking block renderer
		m_pBreakRenderer->SetVisibility(m_IsBreakingBlock);

		if (m_IsBreakingBlock && pBlock)
		{
			// Set the right stage to the breaking renderer
			constexpr int nrBreakStages{ 10 };
			m_pBreakRenderer->SetBreakingStage(std::min(static_cast<int>(m_BlockBreakProgress / pBlock->breakTime * nrBreakStages), nrBreakStages - 1));
		}
	}
	else
	{
		m_pSelection->SetVisibility(false);
		m_pBreakRenderer->SetVisibility(false);
		m_IsBreakingBlock = false;
	}
}

bool BlockInteractionComponent::HasChangedPosition(const XMFLOAT3& position)
{
	bool hasChanged{ abs(position.x - m_PrevPosition.x) > FLT_EPSILON ||
		abs(position.y - m_PrevPosition.y) > FLT_EPSILON ||
		abs(position.z - m_PrevPosition.z) > FLT_EPSILON };

	m_PrevPosition = position;

	return hasChanged;
}
