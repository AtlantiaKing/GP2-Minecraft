#include "stdafx.h"
#include "WorldScene.h"

#include "Components/WorldComponent.h"
#include "Components/PlayerMovement.h"
#include "Components/Rendering/WireframeRenderer.h"
#include "Components/Rendering/BlockBreakRenderer.h"
#include "Components/BlockInteractionComponent.h"
#include "Components/EntityInteractionComponent.h"
#include "Components/Inventory.h"
#include "Components/ToolbarHUD.h"
#include "Components/ItemCounter.h"
#include <Components/Health.h>
#include <Components/HealthHUD.h>
#include "Components/LivingEntities/Sheep.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow.h"

void WorldScene::Initialize()
{
	m_SceneContext.pInput->ForceMouseToCenter(true);
	m_SceneContext.settings.drawPhysXDebug = false;

	CreateWorld();

	m_pSelection = AddChild(new GameObject{});
	m_pSelection->AddComponent(new WireframeRenderer{ m_SceneContext });
	BlockBreakRenderer* pBreakRenderer{ m_pSelection->AddComponent(new BlockBreakRenderer{ m_SceneContext }) };
	pBreakRenderer->SetVisibility(false);
	m_pSelection->GetTransform()->Translate(0.0f, 70.0f, 0.0f);

	CreatePlayer();

	GameObject* pCursor{ AddChild(new GameObject{}) };
	pCursor->AddComponent(new SpriteComponent{ L"Textures\\Crosshair.png", { 0.5f, 0.5f } });
	pCursor->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f, m_SceneContext.windowHeight / 2.0f, 0.0f);

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

	Health* pHealth{ m_pPlayer->AddComponent(new Health{}) };

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

	pHealth->Damage(1);

	for (int i{}; i < 10; ++i)
	{
		GameObject* pSheep{ AddChild(new GameObject{}) };
		pSheep->GetTransform()->Translate(static_cast<float>(rand() % 16), 120.0f, static_cast<float>(rand() % 16));
		pSheep->GetTransform()->Scale(0.02f);

		DiffuseMaterial_Shadow* pSheepMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>() };
		pSheepMaterial->SetDiffuseTexture(L"Textures/Sheep/Sheep.dds");
		pSheep->AddComponent(new ModelComponent{ L"Meshes/Sheep.ovm", true })->SetMaterial(pSheepMaterial);

		pSheep->AddComponent(new Health{ 4 });

		const XMFLOAT3 hitboxHalfDimensions{ 0.25f,0.5f,0.25f };
		pSheep->AddComponent(new Sheep{ hitboxHalfDimensions });

		auto& physX{ PxGetPhysics() };
		auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };
		RigidBodyComponent* pSheepRb{ pSheep->AddComponent(new RigidBodyComponent{}) };
		pSheepRb->AddCollider(PxBoxGeometry{ hitboxHalfDimensions.x,hitboxHalfDimensions.y, hitboxHalfDimensions.z }, *pPhysMat, false, PxTransform{ 0.0f, hitboxHalfDimensions.y, 0.0f });
		pSheepRb->SetConstraint(RigidBodyConstraint::AllRot, false);
		pSheepRb->SetCollisionGroup(CollisionGroup::DefaultCollision | CollisionGroup::LivingEntity);
	}
}

void WorldScene::CreateWorld()
{
	GameObject* pWorld{ AddChild(new GameObject{}) };
	m_pWorld = pWorld->AddComponent(new WorldComponent{ m_SceneContext });
}

void WorldScene::CreatePlayer()
{
	auto& physX{ PxGetPhysics() };
	auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };
	
	// Create player
	m_pPlayer = AddChild(new GameObject{});

	// RIGIDBODY
	RigidBodyComponent* pPlayerRb{ m_pPlayer->AddComponent(new RigidBodyComponent{}) };
	// Add collider
	const PxBoxGeometry playerGeometry{ 0.3f, 1.0f, 0.25f };
	pPlayerRb->AddCollider(playerGeometry, *pPhysMat);
	// Lock all rotations
	pPlayerRb->SetConstraint(RigidBodyConstraint::RotX | RigidBodyConstraint::RotY | RigidBodyConstraint::RotZ, false);
	// Double gravity
	m_PxScene = pPlayerRb->GetPxRigidActor()->getScene();
	m_PxScene->setGravity(m_PxScene->getGravity() * 2);

	// MOVEMENT
	m_pPlayer->AddComponent(new PlayerMovement{ pPlayerRb });

	m_pPlayer->AddComponent(new Inventory{});

	// POSITION
	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
	PxRaycastBuffer hit;
	if (m_PxScene->raycast(PxVec3{ 0.0f,256.0f,0.0f }, PxVec3{ 0.0f,-1.0f,0.0f }, 1000.0f, hit, PxHitFlag::eDEFAULT, filter))
	{
		if (hit.hasBlock)
		{
			const XMFLOAT3 spawnPosition{ hit.block.position.x, hit.block.position.y + playerGeometry.halfExtents.y, hit.block.position.z };
			m_pPlayer->GetTransform()->Translate(spawnPosition);
		}
	}

	// INTERACTION
	PxScene* pxScene{ pPlayerRb->GetPxRigidActor()->getScene() };
	m_pPlayer->AddComponent(new BlockInteractionComponent
		{
			pxScene,
			m_pWorld, 
			m_pSelection->GetComponent<WireframeRenderer>(),
			m_pSelection->GetComponent<BlockBreakRenderer>()
		});
	m_pPlayer->AddComponent(new EntityInteractionComponent{ pxScene });


	// Create camera
	GameObject* pCameraGO{ m_pPlayer->AddChild(new GameObject{}) };

	// CAMERA
	CameraComponent* pCamera{ pCameraGO->AddComponent(new CameraComponent{}) };
	pCamera->SetFieldOfView(XMConvertToRadians(80.0f));
	pCamera->SetFarClippingPlane(150.0f);
	SetActiveCamera(pCamera); //Also sets pCamera in SceneContext

	// POSITION
	pCameraGO->GetTransform()->Translate(0.0f, 0.5f, 0.0f);
}

void WorldScene::Update()
{
	m_pWorld->UpdateColliders(m_pPlayer->GetTransform()->GetWorldPosition());

	const auto& lightDir{m_SceneContext.pLights->GetDirectionalLight().direction};
	const XMFLOAT3 direction{ lightDir.x, lightDir.y, lightDir.z };

	const XMVECTOR directionVec{ XMLoadFloat3(&direction) };
	XMVECTOR positionVec{ XMLoadFloat3(&m_pPlayer->GetTransform()->GetPosition()) };

	positionVec -= directionVec * 150;

	XMFLOAT3 position;
	XMStoreFloat3(&position, positionVec);

	m_SceneContext.pLights->SetDirectionalLight(position, direction);
}

void WorldScene::Draw()
{
	//ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 10.f, 10.f }, { 0.1f, 0.1f }, { 1.f,0.f });
}

void WorldScene::OnGUI()
{
}
