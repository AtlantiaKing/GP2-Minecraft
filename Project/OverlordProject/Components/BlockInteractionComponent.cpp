#include "stdafx.h"
#include "BlockInteractionComponent.h"

#include "WorldComponent.h"
#include "Rendering/WireframeRenderer.h"
#include "Rendering/BlockBreakRenderer.h"

#include "Managers/InputManager.h"
#include "Inventory.h"

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

	// Get camera/ray information
	const XMFLOAT3 cameraPosition{ pCamera->GetWorldPosition() };
	const PxVec3 raycastOrigin{ cameraPosition.x, cameraPosition.y, cameraPosition.z };
	const XMFLOAT3 cameraForward{ pCamera->GetForward() };
	const PxVec3 raycastDirection{ cameraForward.x, cameraForward.y, cameraForward.z };

	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);

	PxRaycastBuffer hit;
	if (!m_PxScene->raycast(raycastOrigin, raycastDirection, playerBlockRadius, hit, PxHitFlag::eDEFAULT, filter))
	{
		// If we are not looking at a block, disable both the break renderer and the selection renderer
		m_pSelection->SetVisibility(false);
		m_pBreakRenderer->SetVisibility(false);

		// Reset block breaking
		m_IsBreakingBlock = false;

		return;
	}

	// If the raycast has no information
	if (!hit.hasBlock)
	{
		// Disable both the break renderer and the selection renderer
		m_pSelection->SetVisibility(false);
		m_pBreakRenderer->SetVisibility(false);
		return;
	}

	// Enable the selection renderer
	m_pSelection->SetVisibility(true);

	// Get the position of the block that we are looking at
	const PxVec3 hitPos{ hit.block.position - hit.block.normal * 0.001f };
	const XMFLOAT3 blockPos
	{
		(hitPos.x + 0.5f) > 0.0f ? floor(hitPos.x + 0.5f) : -(floor(abs(hitPos.x + 0.5f)) + 1),
		(hitPos.y + 0.5f) > 0.0f ? floor(hitPos.y + 0.5f) : -(floor(abs(hitPos.y + 0.5f)) + 1),
		(hitPos.z + 0.5f) > 0.0f ? floor(hitPos.z + 0.5f) : -(floor(abs(hitPos.z + 0.5f)) + 1)
	};
	// Move the selection renderer to the current block position
	m_pSelection->GetTransform()->Translate(blockPos);

	// Get current block
	Block* pBlock{ m_pWorld->GetBlockAt(static_cast<int>(blockPos.x), static_cast<int>(blockPos.y), static_cast<int>(blockPos.z)) };

	// If we changed the block we were looking at, reset block breaking
	if (HasChangedPosition(blockPos)) m_IsBreakingBlock = false;

	// RIGHT MOUSE CLICK
	if (InputManager::IsMouseButton(InputState::pressed, 2))
	{
		Inventory* pInventory{ GetGameObject()->GetComponent<Inventory>() };

		if (pInventory->HasEquipedItem())
		{
			// Get equiped block
			BlockType selectedBlock{ pInventory->GetEquipedItem() };

			// Place a block
			if (m_pWorld->PlaceBlock(XMFLOAT3{ hit.block.normal.x, hit.block.normal.y, hit.block.normal.z }, blockPos, selectedBlock))
			{
				// Reset block breaking
				m_IsBreakingBlock = false;

				pInventory->Remove(selectedBlock);
			}
		}
	}

	// LEFT MOUSE CLICK
	if (InputManager::IsMouseButton(InputState::down, 1))
	{
		// If we are breaking a block
		if (m_IsBreakingBlock && pBlock)
		{
			// Increment breaking time
			m_BlockBreakProgress += sceneContext.pGameTime->GetElapsed();

			// Destroy the block if the progress has reached 100%
			if (m_BlockBreakProgress > pBlock->breakTime)
			{
				m_pWorld->DestroyBlock(blockPos);
			}
		}
		else
		{
			// Destroy the block if it has instant breaking, else reset block breaking
			if (pBlock && pBlock->breakTime <= 0.0f)
			{
				m_pWorld->DestroyBlock(blockPos);
			}
			else
			{
				// Reset breaking block progress
				m_IsBreakingBlock = true;
				m_BlockBreakProgress = 0.0f;
			}
		}
	}
	else
	{
		// Disable block breaking
		m_IsBreakingBlock = false;
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

bool BlockInteractionComponent::HasChangedPosition(const XMFLOAT3& position)
{
	bool hasChanged{ abs(position.x - m_PrevPosition.x) > FLT_EPSILON ||
		abs(position.y - m_PrevPosition.y) > FLT_EPSILON ||
		abs(position.z - m_PrevPosition.z) > FLT_EPSILON };

	m_PrevPosition = position;

	return hasChanged;
}
