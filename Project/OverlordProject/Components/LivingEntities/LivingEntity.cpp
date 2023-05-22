#include "stdafx.h"
#include "LivingEntity.h"
#include <Components/Health.h>

LivingEntity::LivingEntity(const XMFLOAT3& hitboxDimensions)
	: m_HitboxHalfDimensions{ hitboxDimensions }
{
}

void LivingEntity::Notify(const int& health)
{
	m_IsAttacked = true;

	// Notify rotation and state updates
	m_StateTime = m_TimeUntilStateChange;
	m_RotationTime = m_TimeUntilRotation;

	m_AttackTime = 0.0f;

	OnHit(health);
}

void LivingEntity::Initialize(const SceneContext&)
{
	SetNewRotationTimer();
	SetNewStateTimer();

	GetGameObject()->GetComponent<Health>()->OnHealthChange.AddListener(this);
}

void LivingEntity::Update(const SceneContext& sceneContext)
{
	const float elapsedSec{ sceneContext.pGameTime->GetElapsed() };
	m_StateTime += elapsedSec;
	m_RotationTime += elapsedSec;

	if (m_IsAttacked)
	{
		m_AttackTime += elapsedSec;
		if (m_AttackTime > m_TimeUntilRest) m_IsAttacked = false;
	}

	UpdateRotation();

	RootUpdateState();

	Rotate(elapsedSec);

	UpdateMovement(elapsedSec);
}

void LivingEntity::UpdateRotation()
{
	if (m_RotationTime < m_TimeUntilRotation) return;

	SetNewRotationTimer();

	m_GoToRotation = rand() / static_cast<float>(RAND_MAX) * 360.0f;
}

void LivingEntity::RootUpdateState()
{
	if (m_StateTime < m_TimeUntilStateChange) return;

	SetNewStateTimer();

	UpdateState();
}

void LivingEntity::SetNewRotationTimer()
{
	m_RotationTime = 0.0f;
	m_TimeUntilRotation = rand() / static_cast<float>(RAND_MAX) * (m_MaxTimeBetweenRotation - m_MinTimeBetweenRotation) + m_MinTimeBetweenRotation;

	if (m_IsAttacked) m_TimeUntilRotation /= 5;
}

void LivingEntity::SetNewStateTimer()
{
	m_StateTime = 0.0f;
	m_TimeUntilStateChange = rand() / static_cast<float>(RAND_MAX) * (m_MaxTimeBetweenStates - m_MinTimeBetweenStates) + m_MinTimeBetweenStates;

	if (m_IsAttacked) m_TimeUntilStateChange /= 5;
}

void LivingEntity::Rotate(float elapsedSec)
{
	constexpr float rotationEpsilon{ 1.0f };
	if (abs(m_GoToRotation - m_CurRotation) < rotationEpsilon) return;

	float rotateDirection{ m_GoToRotation - m_CurRotation < 0.0f ? -1.0f : 1.0f };

	constexpr float rotationSpeed{ 90.0f };
	m_CurRotation += rotateDirection * rotationSpeed * elapsedSec;

	GetTransform()->Rotate(0.0f, m_CurRotation, 0.0f);
}
