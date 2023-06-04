#include "stdafx.h"
#include "EntityInteractionComponent.h"

#include "BlockInteractionComponent.h"

#include "Health.h"

EntityInteractionComponent::EntityInteractionComponent(PxScene* pxScene)
	: m_PxScene{ pxScene }
{
	// Load sounds
	const auto pFmod{ SoundManager::Get()->GetSystem() };
	for (int i{}; i < m_NrSounds; ++i)
	{
		FMOD::Sound* pSound{ nullptr };

		std::stringstream filePath{};
		filePath << "Resources/Sounds/Hit/Hit" << i << ".mp3";

		FMOD_RESULT result = pFmod->createStream(filePath.str().c_str(), FMOD_DEFAULT, nullptr, &pSound);
		SoundManager::Get()->ErrorCheck(result);

		m_pSounds.push_back(pSound);
	}
}

void EntityInteractionComponent::Initialize(const SceneContext&)
{
	m_pBlockInteraction = GetGameObject()->GetComponent<BlockInteractionComponent>();
}

void EntityInteractionComponent::Update(const SceneContext& sceneContext)
{
	// Do nothing if the input requirement is not met
	if (!InputManager::IsMouseButton(InputState::pressed, 1) && !InputManager::IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_RIGHT_SHOULDER)) return;

	// If we are breaking a block, don't activate the entity interaction
	if (m_pBlockInteraction->IsBreakingBlock()) return;

	//Play the hit sound
	PlayHitSound();	

	// Get camera/ray information
	TransformComponent* pCamera{ sceneContext.pCamera->GetTransform() };
	const XMFLOAT3 cameraPosition{ pCamera->GetWorldPosition() };
	const PxVec3 raycastOrigin{ cameraPosition.x, cameraPosition.y, cameraPosition.z };
	const XMFLOAT3 cameraForward{ pCamera->GetForward() };
	const PxVec3 raycastDirection{ cameraForward.x, cameraForward.y, cameraForward.z };

	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::LivingEntity);

	// If the player hits no entity, stop here
	PxRaycastBuffer hit;
	if (!m_PxScene->raycast(raycastOrigin, raycastDirection, m_PlayerHitRadius, hit, PxHitFlag::eDEFAULT, filter)) return;

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

	// Calculate the knockback force
	XMFLOAT3 knockbackDir{};
	XMStoreFloat3(&knockbackDir, XMLoadFloat3(&pRb->GetTransform()->GetWorldPosition()) - XMLoadFloat3(&GetTransform()->GetWorldPosition()));
	knockbackDir.y = 0.0f;
	XMStoreFloat3(&knockbackDir, XMVector3Normalize(XMLoadFloat3(&knockbackDir)));

	// Apply knockback force
	pRb->AddForce({ knockbackDir.x * 2.0f, 0.0f, knockbackDir.z * 2.0f }, PxForceMode::eIMPULSE);

	// Damage the hit entity
	pEntity->Damage(1);
}

void EntityInteractionComponent::PlayHitSound()
{
	const auto pFmod{ SoundManager::Get()->GetSystem() };
	const FMOD_RESULT result{ pFmod->playSound(m_pSounds[rand() % m_pSounds.size()], nullptr, false, &m_pAudioChannel) };
	SoundManager::Get()->ErrorCheck(result);
	m_pAudioChannel->setVolume(m_HitSoundVolume);
}
