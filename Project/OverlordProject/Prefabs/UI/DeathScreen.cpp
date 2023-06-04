#include "stdafx.h"
#include "DeathScreen.h"

#include "Prefabs/Player.h"
#include "Prefabs/UI/ButtonPrefab.h"

#include "Scenes/WorldScene.h"

#include "Materials/Post/PostDeath.h"

void DeathScreen::Initialize(const SceneContext& sceneContext)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"Fonts/Minecraft_32.fnt");


	constexpr float screenReference{ 1920.0f };
	const float halfWidth{ sceneContext.windowWidth / 2.0f };
	const float uiScale{ sceneContext.windowHeight / screenReference };

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

void DeathScreen::Update(const SceneContext& sceneContext)
{
	const std::wstring header{ L"You died!" };
	constexpr float headerPos{ 100.0f };
	constexpr float pixelSize{ 32.0f / 8.0f };
	constexpr float darkTextColor{ 0.3f };

	TextRenderer::Get()->DrawText(m_pFont, header, { (sceneContext.windowWidth - m_pFont->GetTextSize(header)) / 2.0f + pixelSize, headerPos + pixelSize }, XMFLOAT4{ darkTextColor, darkTextColor, darkTextColor, 1.0f });
	TextRenderer::Get()->DrawText(m_pFont, header, { (sceneContext.windowWidth - m_pFont->GetTextSize(header)) / 2.0f, headerPos });
}

void DeathScreen::OnEnable()
{
	GameScene* pScene{ GetScene() };
	pScene->GetPostProcessingEffect<PostDeath>()->SetIsEnabled(true);
	pScene->GetChild<Player>()->SetActive(false);
	static_cast<WorldScene*>(pScene)->OnPlayerDeath();
}
