#include "stdafx.h"
#include "WorldScene.h"

#include "Components/WorldComponent.h"
#include "Components/PlayerMovement.h"
#include "Components/WireframeRenderer.h"
#include "Components/BlockInteractionComponent.h"

void WorldScene::Initialize()
{
	m_SceneContext.pInput->ForceMouseToCenter(true);
	m_SceneContext.settings.drawPhysXDebug = false;

	auto& physX{ PxGetPhysics() };
	auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };

	m_pSelection = AddChild(new GameObject{});
	m_pSelection->AddComponent(new WireframeRenderer{ m_SceneContext });
	m_pSelection->GetTransform()->Translate(0.0f, 70.0f, 0.0f);

	CreateWorld();

	CreatePlayer(pPhysMat);

	GameObject* pCursor{ AddChild(new GameObject{}) };
	pCursor->AddComponent(new SpriteComponent{ L"Textures\\Crosshair.png", { 0.5f, 0.5f } });
	pCursor->GetTransform()->Translate(m_SceneContext.windowWidth / 2.0f, m_SceneContext.windowHeight / 2.0f, 0.0f);
}

void WorldScene::CreateWorld()
{
	GameObject* pWorld{ AddChild(new GameObject{}) };
	m_pWorld = pWorld->AddComponent(new WorldComponent{ m_SceneContext });
}

void WorldScene::CreatePlayer(physx::PxMaterial* pPhysMat)
{
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
	m_pPlayer->AddComponent(new BlockInteractionComponent{ m_pWorld, m_pSelection->GetComponent<WireframeRenderer>() });


	// Create camera
	GameObject* pCameraGO{ m_pPlayer->AddChild(new GameObject{}) };

	// CAMERA
	CameraComponent* pCamera{ pCameraGO->AddComponent(new CameraComponent{}) };
	pCamera->SetFieldOfView(90.0f);
	SetActiveCamera(pCamera); //Also sets pCamera in SceneContext

	// POSITION
	pCameraGO->GetTransform()->Translate(0.0f, 0.5f, 0.0f);
}

void WorldScene::Update()
{
	TransformComponent* pCamera{ m_SceneContext.pCamera->GetTransform() };
	constexpr float playerBlockRadius{ 5.0f };

	const XMFLOAT3 cameraPosition{ pCamera->GetWorldPosition() };
	const PxVec3 raycastOrigin{ cameraPosition.x, cameraPosition.y, cameraPosition.z };
	const XMFLOAT3 cameraForward{ pCamera->GetForward() };
	const PxVec3 raycastDirection{ cameraForward.x, cameraForward.y, cameraForward.z };

	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);

	PxRaycastBuffer hit;
	if (m_PxScene->raycast(raycastOrigin, raycastDirection, playerBlockRadius, hit, PxHitFlag::eDEFAULT, filter))
	{
		if (!hit.hasBlock)
		{
			m_pSelection->GetComponent<WireframeRenderer>()->SetVisibility(false);
			return;
		}

		m_pSelection->GetComponent<WireframeRenderer>()->SetVisibility(true);

		const PxVec3 hitPos{ hit.block.position + raycastDirection * 0.01f };
		const XMFLOAT3 blockPos
		{
			(hitPos.x + 0.5f) > 0.0f ? floor(hitPos.x + 0.5f) : -(floor(abs(hitPos.x + 0.5f)) + 1),
			(hitPos.y + 0.5f) > 0.0f ? floor(hitPos.y + 0.5f) : -(floor(abs(hitPos.y + 0.5f)) + 1),
			(hitPos.z + 0.5f) > 0.0f ? floor(hitPos.z + 0.5f) : -(floor(abs(hitPos.z + 0.5f)) + 1)
		};
		m_pSelection->GetTransform()->Translate(blockPos);
	}
	else
	{
		m_pSelection->GetComponent<WireframeRenderer>()->SetVisibility(false);
	}


	m_pWorld->UpdateColliders(m_pPlayer->GetTransform()->GetWorldPosition());
}

void WorldScene::Draw()
{
}

void WorldScene::OnGUI()
{
}
