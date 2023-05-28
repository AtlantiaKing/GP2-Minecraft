#include "stdafx.h"
#include "Player.h"

#include "Components/PlayerMovement.h"
#include "Components/Inventory.h"
#include "Components/Rendering/WireframeRenderer.h"
#include "Components/Rendering/BlockBreakRenderer.h"
#include "Components/BlockInteractionComponent.h"
#include "Components/EntityInteractionComponent.h"
#include "Components/WorldComponent.h"
#include "Components/Health.h"

#include "Prefabs/Particles/BlockBreakParticle.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

Player::Player(WorldComponent* pWorld, GameObject* pSelection, BlockBreakParticle* pBlockBreakParticle)
	: m_pWorld{ pWorld }
	, m_pSelection{ pSelection }
	, m_pBlockBreakParticle{ pBlockBreakParticle }
{
}

void Player::Initialize(const SceneContext&)
{
	auto& physX{ PxGetPhysics() };
	auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };

	// RIGIDBODY
	RigidBodyComponent* pPlayerRb{ AddComponent(new RigidBodyComponent{}) };
	// Add collider
	const PxBoxGeometry playerGeometry{ 0.3f, 1.0f, 0.25f };
	pPlayerRb->AddCollider(playerGeometry, *pPhysMat);
	// Lock all rotations
	pPlayerRb->SetConstraint(RigidBodyConstraint::RotX | RigidBodyConstraint::RotY | RigidBodyConstraint::RotZ, false);
	// Double gravity
	PxScene* pxScene{ pPlayerRb->GetPxRigidActor()->getScene() };
	pxScene->setGravity(pxScene->getGravity() * 2);

	// MOVEMENT
	AddComponent(new PlayerMovement{ pPlayerRb });

	AddComponent(new Inventory{});

	// POSITION
	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
	PxRaycastBuffer hit;
	if (pxScene->raycast(PxVec3{ 0.0f, 256.0f, 0.0f }, PxVec3{ 0.0f, -1.0f, 0.0f }, 1000.0f, hit, PxHitFlag::eDEFAULT, filter))
	{
		if (hit.hasBlock)
		{
			const XMFLOAT3 spawnPosition{ hit.block.position.x, hit.block.position.y + playerGeometry.halfExtents.y, hit.block.position.z };
			GetTransform()->Translate(spawnPosition);
		}
	}

	m_pInteraction = AddComponent(new BlockInteractionComponent
		{
			pxScene,
			m_pWorld,
			m_pSelection->GetComponent<WireframeRenderer>(),
			m_pSelection->GetComponent<BlockBreakRenderer>(),
			m_pBlockBreakParticle
		});
	AddComponent(new EntityInteractionComponent{ pxScene });

	AddComponent(new Health{});


	// Create camera
	GameObject* pCameraGO{ AddChild(new GameObject{}) };

	// CAMERA
	CameraComponent* pCamera{ pCameraGO->AddComponent(new CameraComponent{}) };
	pCamera->SetFieldOfView(XMConvertToRadians(80.0f));
	pCamera->SetFarClippingPlane(150.0f);
	pCamera->SetNearClippingPlane(0.01f);
	GetScene()->SetActiveCamera(pCamera); //Also sets pCamera in SceneContext

	// POSITION
	pCameraGO->GetTransform()->Translate(0.0f, 0.5f, 0.0f);

	GameObject* pArm{ pCameraGO->AddChild(new GameObject{}) };

	DiffuseMaterial_Shadow_Skinned* pArmMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>() };
	pArmMaterial->SetDiffuseTexture(L"Textures/Steve.dds");
	ModelComponent* pArmModel{ pArm->AddComponent(new ModelComponent{ L"Meshes/Hand.ovm", false }) };
	pArmModel->SetMaterial(pArmMaterial);

	m_pArmAnimation = pArmModel->GetAnimator();
	m_pArmAnimation->SetAnimationSpeed(5.0f);

	pArm->GetTransform()->Scale(0.005f, 0.006f, 0.003f);
	pArm->GetTransform()->Rotate(20.0f, 190.0f, 0.0f);
	pArm->GetTransform()->Translate(0.2f, -0.17f, 0.09f);
}

void Player::Update(const SceneContext& sceneContext)
{
	m_pWorld->UpdateColliders(GetTransform()->GetWorldPosition());

	if (m_pArmAnimation->DonePlaying())
	{
		m_pArmAnimation->Pause();
	}

	if (m_pInteraction->ShouldPlayAnimation() || sceneContext.pInput->IsMouseButton(InputState::pressed, 1)) m_pArmAnimation->Play();
}
