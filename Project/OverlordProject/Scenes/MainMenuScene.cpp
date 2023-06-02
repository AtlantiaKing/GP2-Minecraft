#include "stdafx.h"
#include "MainMenuScene.h"

#include "Components/WorldComponent.h"

#include "Prefabs/UI/ButtonPrefab.h"

#include "Materials/Post/PostBlur.h"

void MainMenuScene::Initialize()
{
	// 8708
	// 33333
	srand(33333);

	m_SceneContext.pInput->ForceMouseToCenter(false);
	m_SceneContext.settings.drawPhysXDebug = false;

	m_pWorld = new WorldComponent{ m_SceneContext };
#ifndef _DEBUG
	m_pWorld->SetRenderDistance(5);
#endif
	m_pWorld->ShouldLoadAllAtOnce(true);

	AddChild(new GameObject{})->AddComponent(m_pWorld);

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

	constexpr float referenceWidth{ 1920.0f };
	const float screenScale{ m_SceneContext.windowWidth / referenceWidth };

	const float titleHeight{ 230.0f * screenScale };
	const float startButtonPos{ 540.0f * screenScale };
	const float quitButtonPos{ 670.0f * screenScale };

	GameObject* pBackground{ AddChild(new GameObject{}) };
	pBackground->AddComponent(new SpriteComponent{ L"Textures/MainMenu.dds" });
	pBackground->GetTransform()->Scale(screenScale);


	const auto onStartLambda{ [&](const ButtonPrefab&) 
		{
			SceneManager::Get()->SetActiveGameScene(L"World Scene");
		}};
	ButtonPrefab* startButton{ AddChild(new ButtonPrefab{ L"Textures/StartButton.dds" }) };
	startButton->OnClick.AddListener(onStartLambda);

	startButton->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f, startButtonPos, 0.0f);
	startButton->GetTransform()->Scale(screenScale);

	const auto onQuitLambda{ [](const ButtonPrefab&) { PostQuitMessage(0); } };
	ButtonPrefab* quitButton{ AddChild(new ButtonPrefab{ L"Textures/QuitButton.dds" }) };
	quitButton->OnClick.AddListener(onQuitLambda);
	quitButton->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f, quitButtonPos, 0.0f);
	quitButton->GetTransform()->Scale(screenScale);


	GameObject* pTitle{ AddChild(new GameObject{}) };
	pTitle->AddComponent(new SpriteComponent{ L"Textures/Title.dds", { 0.5f, 0.5f } });
	pTitle->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f, titleHeight, 0.0f);
	pTitle->GetTransform()->Scale(screenScale);


	m_pLoadingBackground = AddChild(new GameObject{});
	m_pLoadingBackground->AddComponent(new SpriteComponent{ L"Textures/GeneratingMainMenu.dds" });
	m_pLoadingBackground->GetTransform()->Scale(screenScale);

	// Post Processing Stack
	m_pPostBlur = MaterialManager::Get()->CreateMaterial<PostBlur>();

	AddPostProcessingEffect(m_pPostBlur);
}

void MainMenuScene::Update()
{
	constexpr float rotationSpeed{ 5.0f };
	m_CameraRotation += rotationSpeed * m_SceneContext.pGameTime->GetElapsed();
	if (m_CameraRotation > 360.0f) m_CameraRotation -= 360.0f;
	
	TransformComponent* pCamTransform{ m_SceneContext.pCamera->GetTransform() };

	pCamTransform->Rotate(m_CameraPitch, m_CameraRotation, 0.0f);

	if (m_pLoadingBackground)
	{
		if (m_pWorld->IsLoaded())
		{
			RemoveChild(m_pLoadingBackground, true);
			m_pLoadingBackground = nullptr;
		}
	}
}
