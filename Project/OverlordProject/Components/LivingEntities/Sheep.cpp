#include "stdafx.h"
#include "Sheep.h"

#include "Scenegraph/GameScene.h"
#include "Prefabs/ItemEntity.h"

#include "FmodHelper.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

Sheep::Sheep(const XMFLOAT3& hitboxDimensions)
	: LivingEntity{ hitboxDimensions }
{
	const auto pFmod{ SoundManager::Get()->GetSystem() };

	// Load baa sounds
	for (int i{}; i < m_NrSounds; ++i)
	{
		FMOD::Sound* pSound{ nullptr };

		std::stringstream filePath{};
		filePath << "Resources/Sounds/Sheep/Baa" << i << ".mp3";

		FMOD_RESULT result = pFmod->createStream(filePath.str().c_str(), FMOD_3D | FMOD_3D_LINEARROLLOFF, nullptr, &pSound);
		SoundManager::Get()->ErrorCheck(result);

		m_pSounds.push_back(pSound);
	}

	// Load the walking sound
	FMOD_RESULT result = pFmod->createStream("Resources/Sounds/Sheep/Walk.mp3", FMOD_3D | FMOD_3D_LINEARROLLOFF | FMOD_LOOP_NORMAL, nullptr, &m_pWalkSound);
	SoundManager::Get()->ErrorCheck(result);

	// Start playing the walking sound (paused)
	result = pFmod->playSound(m_pWalkSound, nullptr, true, &m_pWalkAudioChannel);
	SoundManager::Get()->ErrorCheck(result);

	//Set the bounds where the sound can be heard
	m_pWalkAudioChannel->set3DMinMaxDistance(0.f, m_AudioDistance);

	m_pWalkAudioChannel->setVolume(0.3f);
}

Sheep::~Sheep()
{
	m_pWalkAudioChannel->stop();
}

void Sheep::OnHit(int health)
{
	// Play sound
	PlayBaaSound();

	if (health > 0)
	{
		return;
	}

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
		m_pWalkAudioChannel->setPaused(false);
		m_pAnimator->SetAnimation(1 - static_cast<UINT>(m_State));
		m_pAnimator->Play();
		return;
	}

	m_State = rand() % (static_cast<unsigned int>(SheepState::Walking) + 1);

	m_pAnimator->SetAnimation(1 - static_cast<UINT>(m_State));
	m_pAnimator->Play();

	switch (static_cast<SheepState>(m_State))
	{
	case SheepState::Idle:
		m_pWalkAudioChannel->setPaused(true);
		break;
	case SheepState::Walking:
		m_pWalkAudioChannel->setPaused(false);
		break;
	}
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

void Sheep::EntityUpdate(const SceneContext& sceneContext)
{
	m_CurBaaTime += sceneContext.pGameTime->GetElapsed();

	if (m_CurBaaTime > m_CurTimeBetweenBaas)
	{
		m_CurBaaTime -= m_CurTimeBetweenBaas;
		m_CurTimeBetweenBaas = MathHelper::randF(m_TimeBetweenBaasRange.x, m_TimeBetweenBaasRange.y);

		if(!m_IsAttacked) PlayBaaSound();
	}

	//Get the attributes for the source
	auto spherePos = FmodHelper::ToFmod(GetTransform()->GetWorldPosition());
	auto sphereVel = FmodHelper::ToFmod(m_pRb->GetVelocity());

	//Set the attributes for the source
	m_pWalkAudioChannel->set3DAttributes(&spherePos, &sphereVel);
}

void Sheep::PlayBaaSound()
{
	const auto pFmod{ SoundManager::Get()->GetSystem() };
	const FMOD_RESULT result{ pFmod->playSound(m_pSounds[rand() % m_pSounds.size()], nullptr, false, &m_pAudioChannel) };
	SoundManager::Get()->ErrorCheck(result);

	//Set the bounds where the sound can be heard
	m_pAudioChannel->set3DMinMaxDistance(0.f, m_AudioDistance);

	//Get the attributes for the source
	auto spherePos = FmodHelper::ToFmod(GetTransform()->GetWorldPosition());
	auto sphereVel = FmodHelper::ToFmod(m_pRb->GetVelocity());

	//Set the attributes for the source
	m_pAudioChannel->set3DAttributes(&spherePos, &sphereVel);
}
