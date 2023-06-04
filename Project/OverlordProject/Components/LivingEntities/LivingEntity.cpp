#include "stdafx.h"
#include "LivingEntity.h"

#include "Components/Health.h"

#include "Prefabs/Particles/EntityDeathParticle.h"

LivingEntity::LivingEntity(const XMFLOAT3& hitboxDimensions)
	: m_HitboxHalfDimensions{ hitboxDimensions }
{
}

void LivingEntity::Notify(const int& health)
{
	// Spawn a death particle if the entity dies
	if (health == 0)
	{
		GetScene()->AddChild(new EntityDeathParticle{})->GetTransform()->Translate(GetTransform()->GetWorldPosition());
	}

	// Enable the attacked flag
	m_IsAttacked = true;

	// Notify rotation and state updates
	m_StateTime = m_TimeUntilStateChange;
	m_RotationTime = m_TimeUntilRotation;

	// Change the material of the entity to the hit material
	GetGameObject()->GetComponent<ModelComponent>()->SetMaterial(m_pHitMaterial);

	// Reset the attack timer
	m_AttackTime = 0.0f;

	// Call the per-entity-defined OnHit function
	OnHit(health);
}

void LivingEntity::Initialize(const SceneContext&)
{
	// Cache components
	m_pAnimator = GetGameObject()->GetComponent<ModelComponent>()->GetAnimator();
	m_pRb = GetGameObject()->GetComponent<RigidBodyComponent>();

	// Randomize the timers
	SetNewRotationTimer();
	SetNewStateTimer();

	// Add a listener to the health component
	GetGameObject()->GetComponent<Health>()->OnHealthChange.AddListener(this);

	// Initialize materials
	RootInitMaterials();
}

void LivingEntity::Update(const SceneContext& sceneContext)
{
	// Increment timers
	const float elapsedSec{ sceneContext.pGameTime->GetElapsed() };
	m_StateTime += elapsedSec;
	m_RotationTime += elapsedSec;

	// Attack loop
	if (m_IsAttacked)
	{
		// If the material is the hit material
		if (m_AttackTime <= m_TimeUntilDefaultColor)
		{	
			// Reset the material back to the default material
			//		If the attack timer will surpass the timeTillDefaultColor timer
			if (m_AttackTime + elapsedSec > m_TimeUntilDefaultColor)
			{
				GetGameObject()->GetComponent<ModelComponent>()->SetMaterial(m_pDefaultMaterial);
			}
		}

		// Increment attack timer
		m_AttackTime += elapsedSec;

		// Reset attack state if needed
		if (m_AttackTime > m_TimeUntilRest) m_IsAttacked = false;
	}

	// Update entity rotation
	UpdateRotation();

	// Update the entity state
	RootUpdateState();

	// Rotate the entity
	Rotate(elapsedSec);

	// Move the entity (per-entity-defined function)
	UpdateMovement(elapsedSec);

	// Call the per-entity-defined Update call
	EntityUpdate(sceneContext);
}

void LivingEntity::SetHitMaterial(BaseMaterial* pHitMaterial)
{
	m_pHitMaterial = pHitMaterial;
}

void LivingEntity::UpdateRotation()
{
	if (m_RotationTime < m_TimeUntilRotation) return;

	// Randomize the rotation timer if enough time has passed
	SetNewRotationTimer();

	// Set a new go-to rotation
	m_GoToRotation = rand() / static_cast<float>(RAND_MAX) * 360.0f;
}

void LivingEntity::RootUpdateState()
{
	if (m_StateTime < m_TimeUntilStateChange) return;

	// Randomize the state timer if enough time has passed
	SetNewStateTimer();

	// Update the entity state (per-entity-defined function)
	UpdateState();
}

void LivingEntity::SetNewRotationTimer()
{
	// Reset the rotation timer
	m_RotationTime = 0.0f;
	// Randomize the time until new rotation
	m_TimeUntilRotation = rand() / static_cast<float>(RAND_MAX) * (m_MaxTimeBetweenRotation - m_MinTimeBetweenRotation) + m_MinTimeBetweenRotation;

	// Decrease the time between rotation changes if the entity is in attacked mode
	if (m_IsAttacked) m_TimeUntilRotation /= m_AttackedTimeMultiplier;
}

void LivingEntity::SetNewStateTimer()
{
	m_StateTime = 0.0f;
	m_TimeUntilStateChange = rand() / static_cast<float>(RAND_MAX) * (m_MaxTimeBetweenStates - m_MinTimeBetweenStates) + m_MinTimeBetweenStates;

	// Decrease the time between state changes if the entity is in attacked mode
	if (m_IsAttacked) m_TimeUntilStateChange /= m_AttackedTimeMultiplier;
}

void LivingEntity::RootInitMaterials()
{
	// Store the default material
	m_pDefaultMaterial = GetGameObject()->GetComponent<ModelComponent>()->GetMaterial();

	// Call the per-entity-defined function
	InitMaterials();
}

void LivingEntity::Rotate(float elapsedSec)
{
	// Only rotate the entity if the entity has not reached its go-to rotation
	constexpr float rotationEpsilon{ 1.0f };
	if (abs(m_GoToRotation - m_CurRotation) < rotationEpsilon) return;

	// Calculate the direction of the rotation
	float rotateDirection{ m_GoToRotation - m_CurRotation < 0.0f ? -1.0f : 1.0f };

	// Rotate the entity
	m_CurRotation += rotateDirection * m_RotationSpeed * elapsedSec;

	// Rotate the entity
	GetTransform()->Rotate(0.0f, m_CurRotation, 0.0f);
}
