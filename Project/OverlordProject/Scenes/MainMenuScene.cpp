#include "stdafx.h"
#include "MainMenuScene.h"

#include "Components/WorldComponent.h"

void MainMenuScene::Initialize()
{
	// 8708
	// 33333
	srand(33333);

	m_SceneContext.pInput->ForceMouseToCenter(false);
	m_SceneContext.settings.drawPhysXDebug = false;

	WorldComponent* pWorld{ new WorldComponent{ m_SceneContext } };
	pWorld->SetRenderDistance(5);
	AddChild(new GameObject{})->AddComponent(pWorld);



	// Create camera
	GameObject* pCameraGO{ AddChild(new GameObject{}) };

	// CAMERA
	CameraComponent* pCamera{ pCameraGO->AddComponent(new CameraComponent{}) };
	pCamera->SetFieldOfView(XMConvertToRadians(80.0f));
	pCamera->SetFarClippingPlane(150.0f);
	SetActiveCamera(pCamera); //Also sets pCamera in SceneContext

	// POSITION
	TransformComponent* pCamTransform{ pCameraGO->GetTransform() };
	pCamTransform->Translate(0.0f, 80.0f, 0.0f);
	pCamTransform->Rotate(m_CameraPitch, 0.0f, 0.0f);
}

void MainMenuScene::Update()
{
	constexpr float rotationSpeed{ 5.0f };
	m_CameraRotation += rotationSpeed * m_SceneContext.pGameTime->GetElapsed();
	if (m_CameraRotation > 360.0f) m_CameraRotation -= 360.0f;
	
	TransformComponent* pCamTransform{ m_SceneContext.pCamera->GetTransform() };

	pCamTransform->Rotate(m_CameraPitch, m_CameraRotation, 0.0f);
}
