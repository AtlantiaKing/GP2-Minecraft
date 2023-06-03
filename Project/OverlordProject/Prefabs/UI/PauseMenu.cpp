#include "stdafx.h"
#include "PauseMenu.h"

#include "Scenes/WorldScene.h"

#include "Prefabs/UI/ButtonPrefab.h"

#include "Materials/Post/PostDark.h"

PauseMenu::PauseMenu(WorldScene* pScene)
	: m_pScene { pScene }
{
}

void PauseMenu::Initialize(const SceneContext& sceneContext)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"Fonts/Minecraft_32.fnt");

	constexpr float screenReference{ 1920.0f };
	const float halfWidth{ sceneContext.windowWidth / 2.0f };
	const float uiScale{ sceneContext.windowHeight / screenReference };

	ButtonPrefab* pBack{ AddChild(new ButtonPrefab{ L"Textures/BackToGame.dds" }) };
	pBack->GetTransform()->Translate(halfWidth, 250.0f, 0.0f);
	pBack->GetTransform()->Scale(uiScale);
	
	ButtonPrefab* pRestart{ AddChild(new ButtonPrefab{ L"Textures/RestartButton.dds" }) };
	pRestart->GetTransform()->Translate(halfWidth, 300.0f, 0.0f);
	pRestart->GetTransform()->Scale(uiScale);
	pRestart->OnClick.AddListener([this](const ButtonPrefab&) 
		{ 
			WorldScene* pNewScene{ new WorldScene{} };
			SceneManager::Get()->AddGameScene(pNewScene);
			SceneManager::Get()->SetActiveGameScene(pNewScene);
		});
	
	ButtonPrefab* pToTitle{ AddChild(new ButtonPrefab{ L"Textures/QuitToTitle.dds" }) };
	pToTitle->GetTransform()->Translate(halfWidth, 350.0f, 0.0f);
	pToTitle->GetTransform()->Scale(uiScale);
	pToTitle->OnClick.AddListener([](const ButtonPrefab&) { SceneManager::Get()->SetActiveGameScene(L"Main Menu Scene"); });

	ButtonPrefab* pQuit{ AddChild(new ButtonPrefab{ L"Textures/QuitButton.dds" }) };
	pQuit->GetTransform()->Translate(halfWidth, 400.0f, 0.0f);
	pQuit->GetTransform()->Scale(uiScale);
	pQuit->OnClick.AddListener([](const ButtonPrefab&) { PostQuitMessage(0); });
}

void PauseMenu::Update(const SceneContext& sceneContext)
{
	const std::wstring header{ L"Game Menu" };
	constexpr float headerScale{ 0.5f };
	constexpr float headerPos{ 100.0f };

	TextRenderer::Get()->DrawText(m_pFont, header, { (sceneContext.windowWidth - m_pFont->GetTextSize(header) * headerScale ) / 2.0f, headerPos }, XMFLOAT4{ Colors::White }, 0.0f, headerScale);
}

void PauseMenu::OnEnable()
{
	SetPaused(true);
}

void PauseMenu::OnDisable()
{
	SetPaused(false);
}

void PauseMenu::SetPaused(bool paused) const
{
	m_pScene->GetPostProcessingEffect<PostDark>()->SetIsEnabled(paused);
	m_pScene->Pause(paused);
}
