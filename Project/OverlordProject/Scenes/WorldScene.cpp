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
#include "Components/LivingEntities/Sheep.h"

#include "Prefabs/Particles/BlockBreakParticle.h"
#include "Prefabs/Player.h"
#include "Prefabs/UI/PauseMenu.h"
#include "Prefabs/UI/DeathScreen.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"
#include "Materials/SkyBoxMaterial.h"
#include "Materials/Post/PostUnderWater.h"
#include "Materials/Post/PostDark.h"
#include "Materials/Post/PostDeath.h"

void WorldScene::Pause(bool isPaused)
{
	if (m_IsPaused == isPaused) return;

	m_IsPaused = isPaused;

	const auto& children{ GetChildren() };
	for (GameObject* pChild : children)
	{
		if (dynamic_cast<PauseMenu*>(pChild) != nullptr) continue;
		if (dynamic_cast<DeathScreen*>(pChild) != nullptr) continue;
		
		bool disableDraw{};

		if (std::find(begin(m_pGameObjectToHideOnPause), end(m_pGameObjectToHideOnPause), pChild) != end(m_pGameObjectToHideOnPause))
			disableDraw = true;

		pChild->SetActive(!isPaused, disableDraw);
	}

	m_SceneContext.pInput->ForceMouseToCenter(!m_IsPaused);
}

void WorldScene::OnPlayerDeath()
{
	const auto& children{ GetChildren() };
	for (GameObject* pChild : children)
	{
		if (dynamic_cast<DeathScreen*>(pChild) != nullptr) continue;

		if (std::find(begin(m_pGameObjectToHideOnPause), end(m_pGameObjectToHideOnPause), pChild) == end(m_pGameObjectToHideOnPause)) continue;

		pChild->SetActive(false, true);
	}

	m_SceneContext.pInput->ForceMouseToCenter(false);
}

void WorldScene::Initialize()
{
	//m_SceneContext.settings.drawPhysXDebug = false;
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
}

void WorldScene::CreateWorld()
{
	GameObject* pWorld{ AddChild(new GameObject{}) };
	m_pWorld = pWorld->AddComponent(new WorldComponent{ m_SceneContext });
	m_pWorld->LoadStartChunk(m_SceneContext);
}

void WorldScene::Update()
{
	const auto& lightDir{m_SceneContext.pLights->GetDirectionalLight().direction};
	const XMFLOAT3 direction{ lightDir.x, lightDir.y, lightDir.z };

	const XMVECTOR directionVec{ XMLoadFloat3(&direction) };
	XMVECTOR positionVec{ XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()) };

	positionVec -= directionVec * 150;

	XMFLOAT3 position;
	XMStoreFloat3(&position, positionVec);

	m_SceneContext.pLights->SetDirectionalLight(position, direction);

	m_pUnderwater->SetIsEnabled(m_pPlayer->IsUnderWater());

	if (InputManager::IsKeyboardKey(InputState::pressed, VK_ESCAPE))
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
	GameScene* pOtherWorld{ SceneManager::Get()->GetScene(L"World Scene") };
	if (pOtherWorld != this)
	{
		SceneManager::Get()->RemoveGameScene(pOtherWorld, true);
	}

	srand(static_cast<unsigned int>(time(nullptr)));

	m_SceneContext.pInput->ForceMouseToCenter(true);

	CreateWorld();
	m_pWorld->GetGameObject();

	AddChild(new PauseMenu{ this })->SetActive(false);
	AddChild(new DeathScreen{})->SetActive(false);

	// INTERACTION
	BlockBreakParticle* pBlockBreakParticle{ AddChild(new BlockBreakParticle{}) };

	m_pSelection = AddChild(new GameObject{});
	m_pSelection->AddComponent(new WireframeRenderer{ m_SceneContext });
	BlockBreakRenderer* pBreakRenderer{ m_pSelection->AddComponent(new BlockBreakRenderer{ m_SceneContext }) };
	pBreakRenderer->SetVisibility(false);
	m_pSelection->GetTransform()->Translate(0.0f, 70.0f, 0.0f);

	m_pPlayer = AddChild(new Player{ m_pWorld, m_pSelection, pBlockBreakParticle });

	GameObject* pCursor{ AddChild(new GameObject{}) };
	pCursor->AddComponent(new SpriteComponent{ L"Textures\\Crosshair.png", { 0.5f, 0.5f } });
	pCursor->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f, m_SceneContext.windowHeight / 2.0f, 0.0f);
	m_pGameObjectToHideOnPause.push_back(pCursor);

	m_SceneContext.pLights->SetDirectionalLight({ 0.0f,0.0f,0.0f }, { 0.09901f, -0.99015f, 0.09901f });

	Inventory* pInventory{ m_pPlayer->GetComponent<Inventory>() };

	GameObject* pHotbar{ AddChild(new GameObject{}) };
	SpriteComponent* pHotbarTexture{ pHotbar->AddComponent(new SpriteComponent{ L"Textures\\hotbar.dds", { 0.5f, 1.0f } }) };
	ToolbarHUD* pToolbarHud{ pHotbar->AddComponent(new ToolbarHUD{ pInventory, { m_SceneContext.windowWidth, m_SceneContext.windowHeight } }) };
	pHotbar->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f, m_SceneContext.windowHeight, 0.0f);

	const int nrItems{ pInventory->GetMaxItems() };
	constexpr float pixelSize{ 4.0f };
	const auto& hotbarSize{ pHotbarTexture->GetSize() };
	const float itemSize{ (hotbarSize.x - pixelSize * 2.0f) / nrItems };

	for (int i{}; i < nrItems; ++i)
	{
		GameObject* pHotbarItem{ pHotbar->AddChild(new GameObject{}) };
		pHotbarItem->AddComponent(new SpriteComponent{ L"Textures\\InventoryIcons\\air.dds", { 0.0f, 1.0f } });
		pHotbarItem->GetTransform()->Translate(-nrItems / 2.0f * itemSize + itemSize * i, 0.0f, 0.0f);
		GameObject* pHotbarAmount{ pHotbarItem->AddChild(new GameObject{}) };
		pHotbarAmount->AddComponent(new ItemCounter{});
		pHotbarAmount->GetTransform()->Translate(itemSize - pixelSize / 2.0f, -pixelSize, 0.0f);
	}

	GameObject* pHotbarSelection{ AddChild(new GameObject{}) };
	pHotbarSelection->AddComponent(new SpriteComponent{ L"Textures\\inventoryselection.dds", { 0.5f, 1.0f } });
	pHotbarSelection->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f - itemSize * nrItems / 2.0f + itemSize / 2.0f, m_SceneContext.windowHeight, 0.0f);

	pToolbarHud->SetSelection(pHotbarSelection);

	Health* pHealth{ m_pPlayer->GetComponent<Health>() };
	GameObject* pHealthHUD{ AddChild(new GameObject{}) };
	pHealthHUD->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f - hotbarSize.x / 2.0f, m_SceneContext.windowHeight - hotbarSize.y - pixelSize, 0.0f);
	pHealthHUD->AddComponent(new HealthHUD{ pHealth });

	float curHealthPosOffset{};
	for (int i{}; i < pHealth->GetMaxHealth(); ++i)
	{
		GameObject* pHalfHeart{ pHealthHUD->AddChild(new GameObject{}) };

		std::wstringstream spritePath{};
		spritePath << L"Textures/Health/HeartFull" << (i % 2) << L".png";

		SpriteComponent* pHeartSprite{ pHalfHeart->AddComponent(new SpriteComponent{ spritePath.str(), { 0.0f, 1.0f } }) };
		pHalfHeart->GetTransform()->Translate(curHealthPosOffset, 0.0f, 0.0f);
		curHealthPosOffset += pHeartSprite->GetSize().x;
		if (i % 2 == 1) curHealthPosOffset -= pixelSize;
	}

	for (int i{}; i < 1; ++i)
	{
		GameObject* pSheep{ AddChild(new GameObject{}) };
		pSheep->GetTransform()->Translate(static_cast<float>(rand() % 16), 120.0f, static_cast<float>(rand() % 16));
		pSheep->GetTransform()->Scale(0.02f);

		DiffuseMaterial_Shadow_Skinned* pSheepMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>() };
		pSheepMaterial->SetDiffuseTexture(L"Textures/Sheep/Sheep.dds");
		pSheep->AddComponent(new ModelComponent{ L"Meshes/Sheep.ovm", true })->SetMaterial(pSheepMaterial);

		pSheep->AddComponent(new Health{ 4 });

		const XMFLOAT3 hitboxHalfDimensions{ 0.25f,0.5f,0.25f };

		auto& physX{ PxGetPhysics() };
		auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };
		RigidBodyComponent* pSheepRb{ pSheep->AddComponent(new RigidBodyComponent{}) };
		pSheepRb->AddCollider(PxBoxGeometry{ hitboxHalfDimensions.x,hitboxHalfDimensions.y, hitboxHalfDimensions.z }, *pPhysMat, false, PxTransform{ 0.0f, hitboxHalfDimensions.y, 0.0f });
		pSheepRb->SetConstraint(RigidBodyConstraint::AllRot, false);
		pSheepRb->SetCollisionGroup(CollisionGroup::DefaultCollision | CollisionGroup::LivingEntity);

		pSheep->AddComponent(new Sheep{ hitboxHalfDimensions });
	}


	GameObject* pSkybox{ AddChild(new GameObject{}) };
	SkyBoxMaterial* pSkyboxMaterial{ MaterialManager::Get()->CreateMaterial<SkyBoxMaterial>() };
	pSkyboxMaterial->SetTexture(L"Textures/SkyBox.dds");
	pSkybox->AddComponent(new ModelComponent{ L"Meshes/Box.ovm", false })->SetMaterial(pSkyboxMaterial);
	pSkybox->GetTransform()->Scale(100.0f);


	PxScene* pPxScene{ GetPhysxProxy()->GetPhysxScene() };
	pPxScene->setGravity(pPxScene->getGravity() * 2.0f);
}