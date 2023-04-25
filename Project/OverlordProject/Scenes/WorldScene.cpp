#include "stdafx.h"
#include "WorldScene.h"

#include "Components/WorldComponent.h"
#include "Components/PlayerMovement.h"
#include "Components/WireframeRenderer.h"
#include "Components/BlockBreakRenderer.h"
#include "Components/BlockInteractionComponent.h"

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
	m_pPlayer->AddComponent(new BlockInteractionComponent
		{
			pPlayerRb->GetPxRigidActor()->getScene(), 
			m_pWorld, 
			m_pSelection->GetComponent<WireframeRenderer>(),
			m_pSelection->GetComponent<BlockBreakRenderer>()
		});


	// Create camera
	GameObject* pCameraGO{ m_pPlayer->AddChild(new GameObject{}) };

	// CAMERA
	CameraComponent* pCamera{ pCameraGO->AddComponent(new CameraComponent{}) };
	pCamera->SetFieldOfView(80.0f);
	pCamera->SetFarClippingPlane(150.0f);
	SetActiveCamera(pCamera); //Also sets pCamera in SceneContext

	// POSITION
	pCameraGO->GetTransform()->Translate(0.0f, 0.5f, 0.0f);
}

void WorldScene::Update()
{
	m_pWorld->UpdateColliders(m_pPlayer->GetTransform()->GetWorldPosition());
}

void WorldScene::Draw()
{
}

void WorldScene::OnGUI()
{
}
