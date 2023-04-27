#include "stdafx.h"
#include "ItemFloatComponent.h"

void ItemFloatComponent::Update(const SceneContext& sceneContext)
{
	XMFLOAT3 position{ GetTransform()->GetWorldPosition()};
	const PxVec3 raycastOrigin{ position.x, position.y, position.z };
	
	constexpr float moveSpeed{ 2.0f };
	curOffset += sceneContext.pGameTime->GetElapsed() * moveSpeed;
	curOffset = fmodf(curOffset, 2 * PxTwoPi);

	curRotation += sceneContext.pGameTime->GetElapsed();
	curRotation = fmodf(curRotation, 2 * PxTwoPi);

	PxRaycastBuffer hit;
	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
	if (GetGameObject()->GetScene()->GetPhysxProxy()->Raycast(raycastOrigin, PxVec3{0.0f,-1.0f,0.0f}, PX_MAX_F32, hit, PxHitFlag::eDEFAULT, filter))
	{
		if (!hit.hasBlock) return;

		constexpr float floatDistance{ 0.4f };
		constexpr float floatAmplitude{ 0.1f };
		position.y = hit.block.position.y + floatDistance + sinf(curOffset) * floatAmplitude;

		GetTransform()->Translate(position);
	}

	GetTransform()->Rotate(0.0f, XMConvertToDegrees(curRotation), 0.0f);
}
