#include "stdafx.h"
#include "Sheep.h"

#include "Scenegraph/GameScene.h"
#include "Prefabs/ItemEntity.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

Sheep::Sheep(const XMFLOAT3& hitboxDimensions)
	: LivingEntity{ hitboxDimensions }
{
}

void Sheep::OnHit(int health)
{
	if (health > 0) return;

	XMFLOAT3 blockPos{};
	XMStoreFloat3(&blockPos, XMLoadFloat3(&GetTransform()->GetWorldPosition()) + XMVECTOR{ 0.0f, m_HitboxHalfDimensions.y, 0.0f});

	GetScene()->AddChild(new ItemEntity{ BlockType::WOOL, blockPos });
}

void Sheep::InitMaterials()
{
	DiffuseMaterial_Shadow_Skinned* pHitMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>() };
	pHitMaterial->SetDiffuseTexture(L"Textures/Sheep/SheepHit.dds");

	SetHitMaterial(pHitMaterial);
}

void Sheep::UpdateState()
{
	if (m_IsAttacked)
	{
		m_State = static_cast<unsigned int>(SheepState::Walking);
		m_pAnimator->SetAnimation(1 - static_cast<UINT>(m_State));
		m_pAnimator->Play();
		return;
	}

	m_State = rand() % (static_cast<unsigned int>(SheepState::Walking) + 1);

	m_pAnimator->SetAnimation(1 - static_cast<UINT>(m_State));
	m_pAnimator->Play();
}

void Sheep::UpdateMovement(float)
{
	TransformComponent* pTransform{ GetTransform() };
	const auto& forward{ pTransform->GetForward() };
	const auto& worldPos{ pTransform->GetWorldPosition() };

	PxRaycastBuffer hit;
	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
	const PxVec3 raycastOriginCenter{ worldPos.x, worldPos.y + m_HitboxHalfDimensions.y, worldPos.z };

	RigidBodyComponent* rb{ GetGameObject()->GetComponent<RigidBodyComponent>() };

	if (!GetGameObject()->GetScene()->GetPhysxProxy()->Raycast(raycastOriginCenter, PxVec3{ 0.0f,-1.0f,0.0f }, FLT_MAX, hit, PxHitFlag::eDEFAULT, filter))
	{
		rb->SetKinematic(true);
		return;
	}

	rb->SetKinematic(false);

	SheepState sheepState{ static_cast<SheepState>(m_State) };

	if (sheepState == SheepState::Walking)
	{
		const float moveSpeed{ m_RunSpeed + m_AttackedSpeedBoost * m_IsAttacked };

		const XMFLOAT3 direction{ -forward.x * moveSpeed, rb->GetVelocity().y, -forward.z * moveSpeed };

		rb->SetVelocity(direction);
	}
	else
	{
		rb->SetVelocity({0.0f, rb->GetVelocity().y, 0.0f});
	}

	const PxVec3 raycastOriginBottom{ worldPos.x, worldPos.y, worldPos.z };
	if (GetGameObject()->GetScene()->GetPhysxProxy()->Raycast(raycastOriginBottom, PxVec3{ 0.0f,-1.0f,0.0f }, GetRayDistance(), hit, PxHitFlag::eDEFAULT, filter))
	{
		if (!hit.hasBlock) return;

		const PxVec3 raycastOriginForward
		{
			worldPos.x - forward.x * m_HitboxHalfDimensions.x,
			worldPos.y,
			worldPos.z - forward.z * m_HitboxHalfDimensions.z
		};

		if (GetGameObject()->GetScene()->GetPhysxProxy()->Raycast(raycastOriginForward, PxVec3{ -forward.x, -forward.y, -forward.z }, GetRayDistance(), hit, PxHitFlag::eDEFAULT, filter))
		{
			if (!hit.hasBlock) return;

			rb->AddForce({ 0.0f, GetJumpForce(), 0.0f}, PxForceMode::eIMPULSE);
		}
	}
}
