#include "stdafx.h"
#include "ItemEntity.h"

#include "Components/Rendering/BlockRenderer.h"
#include "Components/ItemFloatComponent.h"

ItemEntity::ItemEntity(BlockType block, const XMFLOAT3& blockPos)
	: m_Block{ block }
	, m_Position{ blockPos }
{
}

void ItemEntity::Initialize(const SceneContext& sceneContext)
{
	AddComponent(new BlockRenderer{ m_Block, sceneContext });

	GetTransform()->Scale(0.3f, 0.3f, 0.3f);
	GetTransform()->Translate(m_Position);

	AddComponent(new ItemFloatComponent{});
}
