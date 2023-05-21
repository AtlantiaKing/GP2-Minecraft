#include "stdafx.h"
#include "Sheep.h"

Sheep::Sheep(const XMFLOAT3& hitboxDimensions)
	: LivingEntity{ hitboxDimensions }
{
}

void Sheep::OnHit()
{
	m_State = static_cast<unsigned int>(SheepState::Attacked);
}

void Sheep::UpdateState()
{
	m_State = rand() % (static_cast<unsigned int>(SheepState::Walking) + 1);
}

void Sheep::UpdateMovement(float)
{
	SheepState sheepState{ static_cast<SheepState>(m_State) };

	TransformComponent* pTransform{ GetTransform() };
	const auto& forward{ pTransform->GetForward() };
	const auto& worldPos{ pTransform->GetWorldPosition() };

	if (sheepState == SheepState::Walking)
	{
		auto rb{ GetGameObject()->GetComponent<RigidBodyComponent>() };

		const XMFLOAT3 direction{ -forward.x, rb->GetVelocity().y, -forward.z};

		rb->SetVelocity(direction);
	}
	else
	{
		auto rb{ GetGameObject()->GetComponent<RigidBodyComponent>() };
		rb->SetVelocity({0.0f, rb->GetVelocity().y, 0.0f});
	}

	PxRaycastBuffer hit;
	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
	const PxVec3 raycastOriginBottom{ worldPos.x, worldPos.y, worldPos.z};
	const PxVec3 raycastOriginForward
	{
		worldPos.x - forward.x * m_HitboxHalfDimensions.x,
		worldPos.y,
		worldPos.z - forward.z * m_HitboxHalfDimensions.z
	};
	if (GetGameObject()->GetScene()->GetPhysxProxy()->Raycast(raycastOriginBottom, PxVec3{ 0.0f,-1.0f,0.0f }, GetRayDistance(), hit, PxHitFlag::eDEFAULT, filter))
	{
		if (!hit.hasBlock) return;
		if (GetGameObject()->GetScene()->GetPhysxProxy()->Raycast(raycastOriginForward, PxVec3{ -forward.x, -forward.y, -forward.z }, GetRayDistance(), hit, PxHitFlag::eDEFAULT, filter))
		{
			if (!hit.hasBlock) return;

			RigidBodyComponent* rb{ GetGameObject()->GetComponent<RigidBodyComponent>() };
			rb->AddForce({ 0.0f, GetJumpForce(), 0.0f}, PxForceMode::eIMPULSE);
		}
	}
}
