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
	const auto& joyStickInput{ sceneContext.pInput->GetThumbstickPosition(false) };

	constexpr float joystickMultiplier{ 20.0f };
	constexpr float mouseInputMultiplier{ 0.1f };
	const float horizontalInput{ mouseInput.x * mouseInputMultiplier + joyStickInput.x * joystickMultiplier };
	const float verticalInput{ mouseInput.y * mouseInputMultiplier - joyStickInput.y * joystickMultiplier };

	// Rotate the camera around the local X axis
	TransformComponent* pCameraTransform{ sceneContext.pCamera->GetTransform() };
	if (abs(verticalInput))
	{
		const XMFLOAT4& cameraRotationQuat{ pCameraTransform->GetRotation() };

		XMVECTOR curAxis;
		float curAngle;
		XMQuaternionToAxisAngle(&curAxis, &curAngle, XMLoadFloat4(&cameraRotationQuat));

		curAngle += m_RotateSpeed * verticalInput;

		if (curAngle < 0) curAngle += XM_2PI;
		if (curAngle > XM_2PI) curAngle -= XM_2PI;

		if (curAngle > XM_PIDIV2 && curAngle < XM_PI) 
			curAngle = XM_PIDIV2;
		if (curAngle < XM_2PI - XM_PIDIV2 && curAngle > XM_PI) 
			curAngle = XM_2PI - XM_PIDIV2;

		pCameraTransform->Rotate(XMQuaternionRotationAxis({1.0f,0.0f,0.0f}, curAngle));
	}

	// Rotate the player around the global Y axis
	if (abs(horizontalInput))
	{
		const XMFLOAT4 playerRotationQuat{ m_pPlayer->GetRotation() };
		PxQuat playerQuaternion{ playerRotationQuat.x, playerRotationQuat.y, playerRotationQuat.z, playerRotationQuat.w };
		
		const PxVec3 rotationAxis{ 0.0f,1.0f,0.0f }; // Save as member variable?
		const PxQuat mouseRotation{ m_RotateSpeed * horizontalInput, rotationAxis };

		playerQuaternion = playerQuaternion * mouseRotation;
		const XMFLOAT4 newRotation{ playerQuaternion.x, playerQuaternion.y, playerQuaternion.z, playerQuaternion.w };

		m_pPlayer->Rotate(newRotation);
	}
}

void PlayerMovement::UpdateVelocity(const SceneContext& sceneContext) const
{
	const XMFLOAT3& prevVelocity{ m_pPlayer->GetVelocity() };
	float forwardSpeed;
	XMStoreFloat(&forwardSpeed, XMVector3Dot(XMLoadFloat3(&prevVelocity), XMLoadFloat3(&m_pPlayer->GetTransform()->GetForward())));
	forwardSpeed = std::max(forwardSpeed, 0.0f);

	const float fovChangeOnSprint{ m_SprintFOV - m_FOV };
	const float fovChange{ fovChangeOnSprint / (m_SprintSpeed - m_MoveSpeed) };
	const float gotoFOV{ m_FOV + std::max((forwardSpeed - m_MoveSpeed) * fovChange, 0.0f) };
	const float curFOV{ sceneContext.pCamera->GetFieldOfView() };
	constexpr float fovChangeSpeed{ 10.0f };
	if (abs(curFOV - gotoFOV) > 0.01f)
	{
		sceneContext.pCamera->SetFieldOfView(curFOV + (gotoFOV - curFOV) * sceneContext.pGameTime->GetElapsed() * fovChangeSpeed);
	}
	else
	{
		sceneContext.pCamera->SetFieldOfView(gotoFOV);
	}

	PxScene* physScene{ m_pPlayer->GetPxRigidActor()->getScene() };

	const bool hasForwardInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'Z') || sceneContext.pInput->IsKeyboardKey(InputState::down, 'W') };
	const bool hasBackInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'S') };
	const bool hasRightInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'D') };
	const bool hasLeftInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'Q') || sceneContext.pInput->IsKeyboardKey(InputState::down, 'A') };

	const auto& joyStickInput{ sceneContext.pInput->GetThumbstickPosition(true) };

	const float verticalInput{ static_cast<float>(hasForwardInput - hasBackInput) + joyStickInput.y };
	const float horizontalInput{ static_cast<float>(hasRightInput - hasLeftInput) + joyStickInput.x };

	TransformComponent* pPlayerTransform{ m_pPlayer->GetTransform() };

	XMFLOAT3 velocity{ 0.0f, m_pPlayer->GetVelocity().y, 0.0f };

	const XMFLOAT3 position{ pPlayerTransform->GetWorldPosition() };
	const PxVec3 raycastOrigin{ position.x, position.y - pPlayerTransform->GetWorldScale().y, position.z};

	PxRaycastBuffer hit;
	if (sceneContext.pInput->IsKeyboardKey(InputState::down, ' ') || sceneContext.pInput->IsGamepadButton(InputState::down, XINPUT_GAMEPAD_A))
	{
		PxQueryFilterData filter{};
		filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
		if (physScene->raycast(raycastOrigin, PxVec3{ 0.0f,-1.0f,0.0f }, 0.05f, hit, PxHitFlag::eDEFAULT, filter))
		{
			velocity.y = m_JumpForce;
		}
	}

	bool isSprinting{ (verticalInput > 0.0f) && (sceneContext.pInput->IsKeyboardKey(InputState::down, VK_CONTROL) || sceneContext.pInput->IsGamepadButton(InputState::down, XINPUT_GAMEPAD_LEFT_SHOULDER)) };

	XMVECTOR velocityVec{ XMLoadFloat3(&velocity) };
	const float verticalSpeed{ (verticalInput > 0.0f) ? (isSprinting ? m_SprintSpeed : m_MoveSpeed) : m_MoveSpeed };
	velocityVec += XMLoadFloat3(&pPlayerTransform->GetRight()) * horizontalInput * m_MoveSpeed;
	velocityVec += XMLoadFloat3(&pPlayerTransform->GetForward()) * verticalInput * verticalSpeed;

	XMStoreFloat3(&velocity, velocityVec);

	m_pPlayer->SetVelocity(velocity);
}
