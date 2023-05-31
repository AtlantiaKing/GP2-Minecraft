#include "stdafx.h"
#include "EntityInteractionComponent.h"

#include "Health.h"

EntityInteractionComponent::EntityInteractionComponent(PxScene* pxScene)
	: m_PxScene{ pxScene }
{
}

void EntityInteractionComponent::Update(const SceneContext& sceneContext)
{
	// LEFT MOUSE CLICK
	if (!InputManager::IsMouseButton(InputState::pressed, 1)) return;

	TransformComponent* pCamera{ sceneContext.pCamera->GetTransform() };
	constexpr float playerBlockRadius{ 5.0f };

	// Get camera/ray information
	const XMFLOAT3 cameraPosition{ pCamera->GetWorldPosition() };
	const PxVec3 raycastOrigin{ cameraPosition.x, cameraPosition.y, cameraPosition.z };
	const XMFLOAT3 cameraForward{ pCamera->GetForward() };
	const PxVec3 raycastDirection{ cameraForward.x, cameraForward.y, cameraForward.z };

	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::LivingEntity);

	PxRaycastBuffer hit;
	if (!m_PxScene->raycast(raycastOrigin, raycastDirection, playerBlockRadius, hit, PxHitFlag::eDEFAULT, filter)) return;

	// If the raycast has no information
	if (!hit.hasBlock) return;

	// Retrieve the base component of the rigid actor that has been hit
	BaseComponent* pComponent{ static_cast<BaseComponent*>(hit.block.actor->userData) };

	// Retrieve the entity component of the object that has been hit
	Health* pEntity{ pComponent->GetGameObject()->GetComponent<Health>() };

	if (!pEntity)  return;

	// Get the rigidbody from the hit rigid actor
	RigidBodyComponent* pRb{ reinterpret_cast<RigidBodyComponent*>(pComponent) };
	pRb->AddForce({ 0.0f, 5.0f, 0.0f }, PxForceMode::eIMPULSE);

	XMFLOAT3 knockbackDir{};
	XMStoreFloat3(&knockbackDir, XMLoadFloat3(&pRb->GetTransform()->GetWorldPosition()) - XMLoadFloat3(&GetTransform()->GetWorldPosition()));

	knockbackDir.y = 0.0f;

	XMStoreFloat3(&knockbackDir, XMVector3Normalize(XMLoadFloat3(&knockbackDir)));

	pRb->AddForce({ knockbackDir.x * 2.0f, 0.0f, knockbackDir.z * 2.0f }, PxForceMode::eIMPULSE);

	pEntity->Damage(1);
}
