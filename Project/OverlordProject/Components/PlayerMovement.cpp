#include "stdafx.h"
#include "PlayerMovement.h"

void PlayerMovement::SetUnderWater(bool isUnderWater)
{
	m_IsUnderWater = isUnderWater;
}

void PlayerMovement::AddVelocity(float x, float y, float z)
{
	m_OneFrameVelocity.x += x;
	m_OneFrameVelocity.y += y;
	m_OneFrameVelocity.z += z;
}

void PlayerMovement::Initialize(const SceneContext&)
{
	m_pController = GetGameObject()->GetComponent<ControllerComponent>();
}

void PlayerMovement::Update(const SceneContext& sceneContext)
{
	m_Velocity.x += m_OneFrameVelocity.x;
	m_Velocity.y += m_OneFrameVelocity.y;
	m_Velocity.z += m_OneFrameVelocity.z;
	m_OneFrameVelocity = {};

	PxRaycastBuffer hit;
	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
	const XMFLOAT3& position{ GetTransform()->GetWorldPosition() };
	const PxVec3 raycastOriginCenter{ position.x, 255.0f, position.z };

	if (!GetGameObject()->GetScene()->GetPhysxProxy()->Raycast(raycastOriginCenter, PxVec3{ 0.0f,-1.0f,0.0f }, FLT_MAX, hit, PxHitFlag::eDEFAULT, filter))
	{
		return;
	}

	if (!m_Spawned)
	{
		GetTransform()->Translate(hit.block.position.x, hit.block.position.y + 0.5f, hit.block.position.z);
		m_Spawned = true;
	}

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
		const XMFLOAT4& playerRotationQuat{ GetTransform()->GetRotation()};

		XMVECTOR curAxis;
		float curAngle;
		XMQuaternionToAxisAngle(&curAxis, &curAngle, XMLoadFloat4(&playerRotationQuat));

		curAngle += m_RotateSpeed * horizontalInput;

		if (curAngle < 0) curAngle += XM_2PI;
		if (curAngle > XM_2PI) curAngle -= XM_2PI;

		GetTransform()->Rotate(XMQuaternionRotationAxis({0.0f,1.0f,0.0f}, curAngle));
	}
}

void PlayerMovement::UpdateVelocity(const SceneContext& sceneContext)
{
	float forwardSpeed;
	XMStoreFloat(&forwardSpeed, XMVector3Dot(XMLoadFloat3(&m_Velocity), XMLoadFloat3(&GetTransform()->GetForward())));
	forwardSpeed = std::max(forwardSpeed, 0.0f);

	const float fovChangeOnSprint{ m_SprintFOV - m_FOV };
	const float fovChange{ fovChangeOnSprint / (m_SprintSpeed - m_MoveSpeed) };
	const float gotoFOV{ XMConvertToRadians(m_FOV + std::max((forwardSpeed - m_MoveSpeed) * fovChange, 0.0f)) };
	const float curFOV{ sceneContext.pCamera->GetFieldOfView() };
	constexpr float fovChangeSpeed{ 10.0f };
	if (abs(curFOV - gotoFOV) > 0.01f)
	{
		const float fov{ curFOV + (gotoFOV - curFOV) * sceneContext.pGameTime->GetElapsed() * fovChangeSpeed };
		sceneContext.pCamera->SetFieldOfView(fov);
	}
	else
	{
		sceneContext.pCamera->SetFieldOfView(gotoFOV);
	}

	PxScene* physScene{ GetScene()->GetPhysxProxy()->GetPhysxScene() };

	const bool hasForwardInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'Z') || sceneContext.pInput->IsKeyboardKey(InputState::down, 'W') };
	const bool hasBackInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'S') };
	const bool hasRightInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'D') };
	const bool hasLeftInput{ sceneContext.pInput->IsKeyboardKey(InputState::down, 'Q') || sceneContext.pInput->IsKeyboardKey(InputState::down, 'A') };

	const auto& joyStickInput{ sceneContext.pInput->GetThumbstickPosition(true) };

	const float verticalInput{ static_cast<float>(hasForwardInput - hasBackInput) + joyStickInput.y };
	const float horizontalInput{ static_cast<float>(hasRightInput - hasLeftInput) + joyStickInput.x };

	TransformComponent* pPlayerTransform{ GetTransform() };

	const float defaultGravity{ physScene->getGravity().y };
	const float gravity{ m_IsUnderWater ? m_UnderWaterGravity : defaultGravity };
	m_Velocity = XMFLOAT3{ 0.0f, m_Velocity.y, 0.0f };

	const XMFLOAT3 position{ pPlayerTransform->GetWorldPosition() };
	const PxVec3 raycastOrigin{ position.x, position.y - 0.9f, position.z };

	if (!m_IsUnderWater)
	{
		PxRaycastBuffer hit;
		PxQueryFilterData filter{};
		filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
		if (physScene->raycast(raycastOrigin, PxVec3{ 0.0f,-1.0f,0.0f }, 0.1f, hit, PxHitFlag::eDEFAULT, filter))
		{
			if (sceneContext.pInput->IsKeyboardKey(InputState::down, ' ') || sceneContext.pInput->IsGamepadButton(InputState::down, XINPUT_GAMEPAD_A))
			{
				m_Velocity.y = m_JumpForce;
			}
			else
			{
				m_Velocity.y = gravity * 1.0f;
			}
			m_IsGrounded = true;
		}
		else
		{
			if (m_IsGrounded)
			{
				m_IsGrounded = false;
				if (m_Velocity.y < FLT_EPSILON) m_Velocity.y = 0.0f;
			}
			m_Velocity.y += gravity * sceneContext.pGameTime->GetElapsed();
		}
	}
	else 
	{
		if (sceneContext.pInput->IsKeyboardKey(InputState::down, ' ') || sceneContext.pInput->IsGamepadButton(InputState::down, XINPUT_GAMEPAD_A))
		{
			m_Velocity.y += m_SwimForce * sceneContext.pGameTime->GetElapsed();
		}
		else
		{
			m_Velocity.y += gravity * sceneContext.pGameTime->GetElapsed();
		}

		m_Velocity.y = std::min(m_Velocity.y, m_MaxUnderWaterVelocity);
		m_Velocity.y = std::max(m_Velocity.y, m_MinUnderWaterVelocity);
	}

	bool isSprinting{ (verticalInput > 0.0f) && (sceneContext.pInput->IsKeyboardKey(InputState::down, VK_CONTROL) || sceneContext.pInput->IsGamepadButton(InputState::down, XINPUT_GAMEPAD_LEFT_SHOULDER)) };

	XMVECTOR velocityVec{ XMLoadFloat3(&m_Velocity) };
	const float verticalSpeed{ m_IsUnderWater ? m_SwimSpeed : ((verticalInput > 0.0f) ? (isSprinting ? m_SprintSpeed : m_MoveSpeed) : m_MoveSpeed) };
	velocityVec += XMLoadFloat3(&pPlayerTransform->GetRight()) * horizontalInput * m_MoveSpeed;
	velocityVec += XMLoadFloat3(&pPlayerTransform->GetForward()) * verticalInput * verticalSpeed;

	XMStoreFloat3(&m_Velocity, velocityVec);


	XMVECTOR displacementVec{ velocityVec * sceneContext.pGameTime->GetElapsed() };
	XMFLOAT3 displacement;
	XMStoreFloat3(&displacement, displacementVec);
	
	m_pController->Move(displacement);
}
