#include "stdafx.h"
#include "WorldScene.h"

#include "Components/WorldComponent.h"
#include "Components/Rendering/WireframeRenderer.h"
#include "Components/Rendering/BlockBreakRenderer.h"
#include "Components/Inventory.h"
#include "Components/ToolbarHUD.h"
#include "Components/ItemCounter.h"
#include "Components/Health.h"
#include "Components/HealthHUD.h"

#include "Prefabs/Particles/BlockBreakParticle.h"
#include "Prefabs/Player.h"
#include "Prefabs/UI/PauseMenu.h"
#include "Prefabs/UI/DeathScreen.h"
#include "Prefabs/UI/Achievement.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Materials/SkyBoxMaterial.h"
#include "Materials/Post/PostUnderWater.h"
#include "Materials/Post/PostDark.h"
#include "Materials/Post/PostDeath.h"

void WorldScene::Initialize()
{
	// Disable debug drawing
	m_SceneContext.settings.drawPhysXDebug = false;
	m_SceneContext.settings.showInfoOverlay = false;
	m_SceneContext.settings.drawGrid = false;

	// Post Processing Stack
	m_pUnderwater = MaterialManager::Get()->CreateMaterial<PostUnderWater>();
	AddPostProcessingEffect(m_pUnderwater);
	PostDark* pDark{ MaterialManager::Get()->CreateMaterial<PostDark>() };
	pDark->SetIsEnabled(false);
	AddPostProcessingEffect(pDark);
	PostDeath* pDeath{ MaterialManager::Get()->CreateMaterial<PostDeath>() };
	pDeath->SetIsEnabled(false);
	AddPostProcessingEffect(pDeath);

	// Load music
	SoundManager* pSoundManager{ SoundManager::Get() };
	for (int i{}; i < m_NrSounds; ++i)
	{
		FMOD::Sound* pSound{};
		std::stringstream filePath{};
		filePath << "Resources/Music/music" << i << ".mp3";
		pSoundManager->ErrorCheck(pSoundManager->GetSystem()->createStream(filePath.str().c_str(), FMOD_DEFAULT, nullptr, &pSound));
		m_pSounds.push_back(pSound);
	}

	// Play a song
	PlaySong();
}

void WorldScene::Update()
{
	// Increment the song timer
	m_CurSongTimer += m_SceneContext.pGameTime->GetElapsed();
	// If x minutes have passed, play a song
	if (m_CurSongTimer > m_TimeUntilSong)
	{
		m_CurSongTimer = 0.0f;
		PlaySong();
	}


	// Get the light direction
	const auto& lightDir{m_SceneContext.pLights->GetDirectionalLight().direction};
	const XMFLOAT3 direction{ lightDir.x, lightDir.y, lightDir.z };
	const XMVECTOR directionVec{ XMLoadFloat3(&direction) };

	// Get the player position
	XMVECTOR lightPositionVec{ XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()) };

	// Move the light position backwards
	constexpr float lightDistance{ 150 };
	lightPositionVec -= directionVec * lightDistance;

	// Store the light position
	XMFLOAT3 lightPosition;
	XMStoreFloat3(&lightPosition, lightPositionVec);

	// Apply the new light position
	m_SceneContext.pLights->SetDirectionalLight(lightPosition, direction);


	// Enable/Disable the underwater post processing effect depending if the player is under water or not
	m_pUnderwater->SetIsEnabled(m_pPlayer->IsUnderWater());


	// Enable/Disable the main menu if the pause button is pressed
	if (InputManager::IsKeyboardKey(InputState::pressed, VK_ESCAPE)
		|| InputManager::IsGamepadButton(InputState::pressed, XINPUT_GAMEPAD_START))
	{
		PauseMenu* pPauseMenu{ GetChild<PauseMenu>() };
		pPauseMenu->SetActive(!pPauseMenu->IsEnabled(), true);
	}
}

void WorldScene::Draw()
{
	//ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 10.f, 10.f }, { 0.1f, 0.1f }, { 1.f,0.f });
}

void WorldScene::OnGUI()
{
}

void WorldScene::OnSceneActivated()
{
	// Delete the previous world if one exists
	GameScene* pOtherWorld{ SceneManager::Get()->GetScene(L"World Scene") };
	if (pOtherWorld != this)
	{
		SceneManager::Get()->RemoveGameScene(pOtherWorld, true);
	}

	// Set a random seed
	srand(static_cast<unsigned int>(time(nullptr)));

	// Disable the mouse cursor
	m_SceneContext.pInput->ForceMouseToCenter(true);

	// Create the world object
	CreateWorld();

	// Create UI elements
	CreateUI();

	// Create particles and block outlines
	CreatePlayerFeedback();

	// Create player
	m_pPlayer = AddChild(new Player{ m_pWorld, m_pSelection, m_pBlockBreakParticle });

	// Initialize the directional light
	m_SceneContext.pLights->SetDirectionalLight({ 0.0f,0.0f,0.0f }, { 0.09901f, -0.99015f, 0.09901f });

	// Create HUD
	CreateHUD();

	// Create skybox
	CreateSkyBox();

	// Double the gravity in the scene
	PxScene* pPxScene{ GetPhysxProxy()->GetPhysxScene() };
	pPxScene->setGravity(pPxScene->getGravity() * 2.0f);
}

void WorldScene::CreateWorld()
{
	// Create the world
	GameObject* pWorld{ AddChild(new GameObject{}) };
	m_pWorld = pWorld->AddComponent(new WorldComponent{ m_SceneContext });
	// Load the first 3x3 chunks on the main thread
	m_pWorld->LoadStartChunk(m_SceneContext);
}

void WorldScene::CreateUI()
{
	AddChild(new PauseMenu{ this })->SetActive(false);
	AddChild(new DeathScreen{})->SetActive(false);
	AddChild(new Achievement{ L"Textures/AchievementHint.dds" })->Show();
}

void WorldScene::CreatePlayerFeedback()
{
	// Create particles
	m_pBlockBreakParticle = AddChild(new BlockBreakParticle{});

	// Create block outline & breaking effect
	m_pSelection = AddChild(new GameObject{});
	m_pSelection->AddComponent(new WireframeRenderer{ m_SceneContext });
	BlockBreakRenderer* pBreakRenderer{ m_pSelection->AddComponent(new BlockBreakRenderer{ m_SceneContext }) };
	pBreakRenderer->SetVisibility(false);
}

void WorldScene::CreateHUD()
{
	// Create crosshair
	GameObject* pCursor{ AddChild(new GameObject{}) };
	pCursor->AddComponent(new SpriteComponent{ L"Textures\\Crosshair.png",{ 0.5f, 0.5f } });
	pCursor->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f, m_SceneContext.windowHeight / 2.0f, 0.0f);
	m_pGameObjectToHideOnPause.push_back(pCursor); // This object should be hidden if the game is paused

	// Get the player inventory
	Inventory* pInventory{ m_pPlayer->GetComponent<Inventory>() };

	// Create hotbar gameobject
	GameObject* pHotbar{ AddChild(new GameObject{}) };
	SpriteComponent* pHotbarTexture{ pHotbar->AddComponent(new SpriteComponent{ L"Textures\\hotbar.dds", { 0.5f, 1.0f } }) };
	ToolbarHUD* pToolbarHud{ pHotbar->AddComponent(new ToolbarHUD{ pInventory,{ m_SceneContext.windowWidth, m_SceneContext.windowHeight } }) };
	pHotbar->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f, m_SceneContext.windowHeight, 0.0f);

	// Populate the hotbar with items
	const int nrItems{ pInventory->GetMaxItems() };
	constexpr float pixelSize{ 4.0f };
	const auto& hotbarSize{ pHotbarTexture->GetSize() };
	const float itemSize{ (hotbarSize.x - pixelSize * 2.0f) / nrItems };

	for (int i{}; i < nrItems; ++i)
	{
		GameObject* pHotbarItem{ pHotbar->AddChild(new GameObject{}) };
		pHotbarItem->AddComponent(new SpriteComponent{ L"Textures\\InventoryIcons\\air.dds",{ 0.0f, 1.0f } });
		pHotbarItem->GetTransform()->Translate(-nrItems / 2.0f * itemSize + itemSize * i, 0.0f, 0.0f);
		GameObject* pHotbarAmount{ pHotbarItem->AddChild(new GameObject{}) };
		pHotbarAmount->AddComponent(new ItemCounter{}); // Each item in the hotbar has an itemcounter
		pHotbarAmount->GetTransform()->Translate(itemSize - pixelSize / 2.0f, -pixelSize, 0.0f);
	}

	// Create hotbar selection object
	GameObject* pHotbarSelection{ AddChild(new GameObject{}) };
	pHotbarSelection->AddComponent(new SpriteComponent{ L"Textures\\inventoryselection.dds",{ 0.5f, 1.0f } });
	pHotbarSelection->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f - itemSize * nrItems / 2.0f + itemSize / 2.0f, m_SceneContext.windowHeight, 0.0f);

	// Apply the selection object to the toolbar hud
	pToolbarHud->SetSelection(pHotbarSelection);

	// Get the player health
	Health* pHealth{ m_pPlayer->GetComponent<Health>() };

	// Create health HUD object
	GameObject* pHealthHUD{ AddChild(new GameObject{}) };
	pHealthHUD->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f - hotbarSize.x / 2.0f, m_SceneContext.windowHeight - hotbarSize.y - pixelSize, 0.0f);
	pHealthHUD->AddComponent(new HealthHUD{ pHealth });

	// Populate the health HUD with hearts
	float curHealthPosOffset{};
	for (int i{}; i < pHealth->GetMaxHealth(); ++i)
	{
		GameObject* pHalfHeart{ pHealthHUD->AddChild(new GameObject{}) };

		std::wstringstream spritePath{};
		spritePath << L"Textures/Health/HeartFull" << (i % 2) << L".png";

		SpriteComponent* pHeartSprite{ pHalfHeart->AddComponent(new SpriteComponent{ spritePath.str(),{ 0.0f, 1.0f } }) };
		pHalfHeart->GetTransform()->Translate(curHealthPosOffset, 0.0f, 0.0f);
		curHealthPosOffset += pHeartSprite->GetSize().x;
		// Each heart should have one pixel in common, so we make sure each first sprite of a heart is offsetted by one pixel
		if (i % 2 == 1) curHealthPosOffset -= pixelSize;
	}
}

void WorldScene::CreateSkyBox()
{
	GameObject* pSkybox{ AddChild(new GameObject{}) };
	SkyBoxMaterial* pSkyboxMaterial{ MaterialManager::Get()->CreateMaterial<SkyBoxMaterial>() };
	pSkyboxMaterial->SetTexture(L"Textures/SkyBox.dds");
	pSkybox->AddComponent(new ModelComponent{ L"Meshes/Box.ovm", false })->SetMaterial(pSkyboxMaterial);
}

void WorldScene::PlaySong()
{
	// Play a random song from the list
	SoundManager::Get()->GetSystem()->playSound(m_pSounds[rand() % m_pSounds.size()], nullptr, false, &m_pChannel);
}

void WorldScene::Pause(bool isPaused)
{
	if (m_IsPaused == isPaused) return;

	m_IsPaused = isPaused;

	// Disable al gameobjects that are not the pausemenu or death screen
	const auto& children{ GetChildren() };
	for (GameObject* pChild : children)
	{
		if (dynamic_cast<PauseMenu*>(pChild) != nullptr) continue;
		if (dynamic_cast<DeathScreen*>(pChild) != nullptr) continue;

		bool disableDraw{};

		// Some objects should also be hidden from the game
		if (std::find(begin(m_pGameObjectToHideOnPause), end(m_pGameObjectToHideOnPause), pChild) != end(m_pGameObjectToHideOnPause))
			disableDraw = true;

		pChild->SetActive(!isPaused, disableDraw);
	}

	// Hide/Show the cursor depending on the pause mode
	m_SceneContext.pInput->ForceMouseToCenter(!m_IsPaused);
}

void WorldScene::OnPlayerDeath()
{
	// Hide set objects from the game
	const auto& children{ GetChildren() };
	for (GameObject* pChild : children)
	{
		if (dynamic_cast<DeathScreen*>(pChild) != nullptr) continue;

		if (std::find(begin(m_pGameObjectToHideOnPause), end(m_pGameObjectToHideOnPause), pChild) == end(m_pGameObjectToHideOnPause)) continue;

		pChild->SetActive(false, true);
	}

	// Show the cursor on death
	m_SceneContext.pInput->ForceMouseToCenter(false);
}

void WorldScene::OnSceneDeactivated()
{
	m_pChannel->stop();
}
