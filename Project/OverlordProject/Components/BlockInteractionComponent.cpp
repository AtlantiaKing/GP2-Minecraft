#include "stdafx.h"
#include "BlockInteractionComponent.h"

#include "WorldComponent.h"
#include "Rendering/WireframeRenderer.h"
#include "Rendering/BlockBreakRenderer.h"

#include "Prefabs/Particles/BlockBreakParticle.h"

#include "Managers/InputManager.h"
#include "Inventory.h"

BlockInteractionComponent::BlockInteractionComponent(PxScene* pxScene, WorldComponent* pWorld, WireframeRenderer* pSelection, BlockBreakRenderer* pBreakRenderer, BlockBreakParticle* pBlockBreakParticle)
	: m_pWorld{ pWorld }
	, m_pSelection{ pSelection }
	, m_PxScene{ pxScene }
	, m_pBreakRenderer{ pBreakRenderer }
	, m_pBlockBreakParticle{ pBlockBreakParticle }
{
}

bool BlockInteractionComponent::ShouldPlayAnimation()
{
	return m_ShouldPlayAnimation;
}

void BlockInteractionComponent::Update(const SceneContext& sceneContext)
{
	// Disable the arm animation
	m_ShouldPlayAnimation = false;

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

		// Disable the block break particle
		m_pBlockBreakParticle->SetActive(false);

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

		// Disable the block break particle
		m_pBlockBreakParticle->SetActive(false);
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
	const XMINT3 blockPosInt{ static_cast<int>(blockPos.x), static_cast<int>(blockPos.y), static_cast<int>(blockPos.z) };
	Block* pBlock{ m_pWorld->GetBlockAt(blockPosInt.x, blockPosInt.y, blockPosInt.z) };

	// If we changed the block we were looking at, reset block breaking
	if (HasChangedPosition(blockPos)) m_IsBreakingBlock = false;

	// RIGHT MOUSE CLICK
	if (InputManager::IsMouseButton(InputState::down, 2))
	{
		// Get inventory
		Inventory* pInventory{ GetGameObject()->GetComponent<Inventory>() };

		// If the inventory has an item equiped in the current slot
		if (pInventory->HasEquipedItem())
		{
			// Get equiped block
			BlockType selectedBlock{ pInventory->GetEquipedItem() };

			// If the block is not inside the player
			if (!IsBlockInPlayer(blockPosInt, hit.block.normal))
			{
				// Try placing a block
				if (m_pWorld->PlaceBlock(XMFLOAT3{ hit.block.normal.x, hit.block.normal.y, hit.block.normal.z }, blockPos, selectedBlock))
				{
					// Reset block breaking
					m_IsBreakingBlock = false;

					// Remove the current block from the inventory
					pInventory->Remove(selectedBlock);

					// Play the arm animation
					m_ShouldPlayAnimation = true;
				}
			}
		}
	}

	// LEFT MOUSE CLICK
	if (InputManager::IsMouseButton(InputState::down, 1))
	{
		// If we are breaking a block
		if (m_IsBreakingBlock && pBlock)
		{
			// Play the arm animation
			m_ShouldPlayAnimation = true;

			// Increment breaking time
			m_BlockBreakProgress += sceneContext.pGameTime->GetElapsed();

			// Enable the block break particle
			m_pBlockBreakParticle->SetActive(true);
			TransformComponent* pParticleTransform{ m_pBlockBreakParticle->GetTransform() };
			pParticleTransform->Translate(blockPos);

			XMFLOAT4 lookAtQuaternion{ MathHelper::GetLookAtQuaternion({ hit.block.normal.x, hit.block.normal.y, hit.block.normal.z }) };
			pParticleTransform->Rotate(XMLoadFloat4(&lookAtQuaternion));

			m_pBlockBreakParticle->SetBlock(pBlock->dropBlock ? pBlock->dropBlock->type : pBlock->type);

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

				// Play the arm animation
				m_ShouldPlayAnimation = true;
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

		// Disable the block break particle
		m_pBlockBreakParticle->SetActive(false);
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

bool BlockInteractionComponent::IsBlockInPlayer(XMINT3 hitBlock, const PxVec3& hitNormal) const
{
	// Get the player position
	const auto& curPlayerPos{ GetTransform()->GetWorldPosition() };
	const XMFLOAT3 playerPosFloored
	{
		(curPlayerPos.x + 0.5f) > 0.0f ? floor(curPlayerPos.x + 0.5f) : -(floor(abs(curPlayerPos.x + 0.5f)) + 1),
		curPlayerPos.y,
		(curPlayerPos.z + 0.5f) > 0.0f ? floor(curPlayerPos.z + 0.5f) : -(floor(abs(curPlayerPos.z + 0.5f)) + 1)
	};
	const XMINT3 playerPosInt{ static_cast<int>(playerPosFloored.x), static_cast<int>(playerPosFloored.y), static_cast<int>(playerPosFloored.z) };

	hitBlock.x = static_cast<int>(hitBlock.x + hitNormal.x);
	hitBlock.y = static_cast<int>(hitBlock.y + hitNormal.y);
	hitBlock.z = static_cast<int>(hitBlock.z + hitNormal.z);

	// Check whether or not the block to be placed is inside 
	return playerPosInt.x == hitBlock.x && playerPosInt.z == hitBlock.z && (playerPosInt.y == hitBlock.y || playerPosInt.y + 1 == hitBlock.y);
}

bool BlockInteractionComponent::HasChangedPosition(const XMFLOAT3& position)
{
	bool hasChanged{ abs(position.x - m_PrevPosition.x) > FLT_EPSILON ||
		abs(position.y - m_PrevPosition.y) > FLT_EPSILON ||
		abs(position.z - m_PrevPosition.z) > FLT_EPSILON };

	m_PrevPosition = position;

	return hasChanged;
}
