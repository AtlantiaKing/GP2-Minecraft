#include "stdafx.h"
#include "ItemEntity.h"

#include "Components/Rendering/BlockRenderer.h"
#include "Components/ItemFloatComponent.h"
#include "Components/ItemPickup.h"

ItemEntity::ItemEntity(BlockType block, const XMFLOAT3& blockPos)
	: m_Block{ block }
	, m_Position{ blockPos }
{
}

void ItemEntity::Initialize(const SceneContext& sceneContext)
{
	AddComponent(new BlockRenderer{ m_Block, sceneContext });

	const float blockSize{ 0.3f };
	GetTransform()->Scale(blockSize, blockSize, blockSize);
	GetTransform()->Translate(m_Position);

	AddComponent(new ItemFloatComponent{});

	RigidBodyComponent* pRb{ AddComponent(new RigidBodyComponent(true)) };

	auto& physX{ PxGetPhysics() };
	const auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };

	const PxBoxGeometry geo{ { blockSize / 2.0f, blockSize / 2.0f, blockSize / 2.0f } };

	pRb->AddCollider(geo, *pPhysMat, true);

	AddComponent(new ItemPickup{ m_Block });
}
