#include "stdafx.h"
#include "PlayerMovement.h"

PlayerMovement::PlayerMovement(RigidBodyComponent* pPlayer)
	: m_pPlayer{ pPlayer }
{
}

void PlayerMovement::Initialize(const SceneContext& /*sceneContext*/)
{
}

void PlayerMovement::Update(const SceneContext& sceneContext)
{
	UpdateRotation(sceneContext);

	UpdateVelocity(sceneContext);
}

void PlayerMovement::UpdateRotation(const SceneContext& sceneContext) const
{
	// Get mouse input
	const auto& mouseInput{ sceneContext.pInput->GetMouseMovement() };

	// Rotate the camera around the local X axis
	TransformComponent* pCameraTransform{ sceneContext.pCamera->GetTransform() };
	if (abs(mouseInput.y))
	{
		const XMFLOAT4 cameraRotationQuat{ pCameraTransform->GetRotation() };
		XMVECTOR cameraRotation{ XMLoadFloat4(&cameraRotationQuat) };
		
		constexpr XMVECTOR rotationAxis{ 1.0f,0.0f,0.0f };
		cameraRotation = XMQuaternionMultiply(cameraRotation, XMQuaternionRotationAxis(rotationAxis, m_RotateSpeed * mouseInput.y));

		pCameraTransform->Rotate(cameraRotation);
	}

	// Rotate the player around the global Y axis
	if (abs(mouseInput.x))
	{
		const XMFLOAT4 playerRotationQuat{ m_pPlayer->GetRotation() };
		PxQuat playerQuaternion{ playerRotationQuat.x, playerRotationQuat.y, playerRotationQuat.z, playerRotationQuat.w };
		
		const PxVec3 rotationAxis{ 0.0f,1.0f,0.0f }; // Save as member variable?
		const PxQuat mouseRotation{ m_RotateSpeed * mouseInput.x, rotationAxis };

		playerQuaternion = playerQuaternion * mouseRotation;
		const XMFLOAT4 newRotation{ playerQuaternion.x, playerQuaternion.y, playerQuaternion.z, playerQuaternion.w };

		m_pPlayer->Rotate(newRotation);
	}
}

void PlayerMovement::UpdateVelocity(const SceneContext& sceneContext) const
{
	PxScene* physScene{ m_pPlayer->GetPxRigidActor()->getScene() };

	const bool hasForwardInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'Z') || sceneContext.pInput->IsKeyboardKey(InputState::down, 'W') };
	const bool hasBackInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'S') };
	const bool hasRightInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'D') };
	const bool hasLeftInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'Q') || sceneContext.pInput->IsKeyboardKey(InputState::down, 'A') };

	const float verticalInput{ static_cast<float>(hasForwardInput - hasBackInput) };
	const float horizontalInput{ static_cast<float>(hasRightInput - hasLeftInput) };

	TransformComponent* pPlayerTransform{ m_pPlayer->GetTransform() };

	XMFLOAT3 velocity{ 0.0f, m_pPlayer->GetVelocity().y, 0.0f };

	const XMFLOAT3 position{ pPlayerTransform->GetWorldPosition() };
	const PxVec3 raycastOrigin{ position.x, position.y - pPlayerTransform->GetWorldScale().y, position.z};

	PxRaycastBuffer hit;
	if (sceneContext.pInput->IsKeyboardKey(InputState::down, ' '))
	{
		PxQueryFilterData filter{};
		filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
		if (physScene->raycast(raycastOrigin, PxVec3{ 0.0f,-1.0f,0.0f }, 0.05f, hit, PxHitFlag::eDEFAULT, filter))
		{
			velocity.y = m_JumpForce;
		}
	}

	XMVECTOR velocityVec{ XMLoadFloat3(&velocity) };
	velocityVec += XMLoadFloat3(&pPlayerTransform->GetRight()) * horizontalInput * m_MoveSpeed;
	velocityVec += XMLoadFloat3(&pPlayerTransform->GetForward()) * verticalInput * m_MoveSpeed;

	XMStoreFloat3(&velocity, velocityVec);

	m_pPlayer->SetVelocity(velocity);
}
