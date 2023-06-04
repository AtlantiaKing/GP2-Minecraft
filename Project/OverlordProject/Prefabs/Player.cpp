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
#include "Components/ControllerComponent.h"
#include "Components/PlayerMovement.h"

#include "Prefabs/Particles/BlockBreakParticle.h"
#include "Prefabs/UI/DeathScreen.h"
#include "Prefabs/UI/Achievement.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

Player::Player(WorldComponent* pWorld, GameObject* pSelection, BlockBreakParticle* pBlockBreakParticle)
	: m_pWorld{ pWorld }
	, m_pSelection{ pSelection }
	, m_pBlockBreakParticle{ pBlockBreakParticle }
{
}

bool Player::IsUnderWater() const
{
	return m_IsCamUnderWater;
}

void Player::Initialize(const SceneContext&)
{
	auto& physX{ PxGetPhysics() };
	auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };

	PxCapsuleControllerDesc controller{};
	controller.radius = 0.4f;
	controller.height = 1.0f;
	controller.material = pPhysMat;

	ControllerComponent* pController{ AddComponent(new ControllerComponent(controller)) };
	pController->SetStepHeight(0.0f);

	// MOVEMENT
	m_pMovement = AddComponent(new PlayerMovement{});

	AddComponent(new Inventory{})->OnInventoryChange().AddListener([this](const std::unordered_map<BlockType, int> inventory)
		{
			static bool hasAchievement{};
			if (hasAchievement) return;

			if (!inventory.contains(BlockType::WOOL)) return;

			const int amountWool{ inventory.at(BlockType::WOOL) };
			if (amountWool < 10) return;

			hasAchievement = true;
			Achievement* pAchievement{ GetScene()->GetChild<Achievement>() };
			pAchievement->SetTexture(L"Textures/AchievementMade.dds");
			pAchievement->Show();
		});

	// POSITION
	PxQueryFilterData filter{};
	filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
	PxRaycastBuffer hit;
	PxScene* pxScene{ GetScene()->GetPhysxProxy()->GetPhysxScene() };
	if (pxScene->raycast(PxVec3{ 0.0f, 256.0f, 0.0f }, PxVec3{ 0.0f, -1.0f, 0.0f }, 1000.0f, hit, PxHitFlag::eDEFAULT, filter))
	{
		if (hit.hasBlock)
		{
			const XMFLOAT3 spawnPosition{ hit.block.position.x, hit.block.position.y + controller.height / 2.0f, hit.block.position.z };
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




	Health* pHealth{ AddComponent(new Health{}) };
	pHealth->OnDeath.AddListener([this](const GameObject&) { GetScene()->GetChild<DeathScreen>()->SetActive(true); });
	pHealth->SetDestroyOnDeath(false);
	pHealth->OnHealthChange.AddListener([this](int) 
		{
			SoundManager::Get()->GetSystem()->playSound(m_pDamageSound, nullptr, false, &m_pDamageChannel);
		});



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




	m_pFeet = GetScene()->AddChild(new GameObject{});
	RigidBodyComponent* pFeetRb{ m_pFeet->AddComponent(new RigidBodyComponent{}) };
	pFeetRb->AddCollider(PxBoxGeometry{ 0.3f, 0.05f, 0.3f }, *pPhysMat, true);
	pFeetRb->SetCollisionIgnoreGroups(~CollisionGroup::World);



	m_pFeet->SetOnTriggerCallBack([this](GameObject* pObject, GameObject* pOther, PxTriggerAction action)
		{
			if (pOther->GetComponent<WorldComponent>() == nullptr) return;

			static float startDistance{};

			const float curHeight{ pObject->GetTransform()->GetWorldPosition().y };

			PxRaycastBuffer hit;
			PxQueryFilterData filter{};
			filter.data.word0 = static_cast<PxU32>(CollisionGroup::World);
			XMFLOAT3 position{ pObject->GetTransform()->GetWorldPosition() };
			position.y += 0.05f;

			if (action == PxTriggerAction::LEAVE)
			{
				if (!GetScene()->GetPhysxProxy()->Raycast(PhysxHelper::ToPxVec3(position), PxVec3{ 0.0f,-1.0f,0.0f }, 0.1f, hit, PxHitFlag::eDEFAULT, filter))
				{
					startDistance = curHeight;
				}
			}
			else
			{
				GetScene()->GetPhysxProxy()->Raycast(PhysxHelper::ToPxVec3(position), PxVec3{ 0.0f,-1.0f,0.0f }, FLT_MAX, hit, PxHitFlag::eDEFAULT, filter);

				if (hit.block.normal.y < 0.5f) return;

				const float fallingDistance{ startDistance - curHeight };
				constexpr float startFallDamageHeight{ 3.0f };

				if (fallingDistance - startFallDamageHeight > 0.0f)
				{
					GetComponent<Health>()->Damage(static_cast<int>(fallingDistance - startFallDamageHeight));
					SoundManager::Get()->GetSystem()->playSound(m_pFallSound, nullptr, false, &m_pFallSoundChannel);
				}
			}
		}
	);



	FMOD_RESULT result{ SoundManager::Get()->GetSystem()->createStream("Resources/Sounds/Damage/fall.ogg", FMOD_DEFAULT, nullptr, &m_pFallSound) };
	SoundManager::Get()->ErrorCheck(result);

	result = SoundManager::Get()->GetSystem()->createStream("Resources/Sounds/Damage/damage.ogg", FMOD_DEFAULT, nullptr, &m_pDamageSound);
	SoundManager::Get()->ErrorCheck(result);
}

void Player::Update(const SceneContext& sceneContext)
{
	m_pWorld->UpdateColliders(GetTransform()->GetWorldPosition());

	if (m_pArmAnimation->DonePlaying())
	{
		m_pArmAnimation->Pause();
	}

	if (m_pInteraction->ShouldPlayAnimation() || sceneContext.pInput->IsMouseButton(InputState::pressed, 1)) m_pArmAnimation->Play();

	bool prevUnderwater{ m_pMovement->IsSwimming() };
	bool isUnderWater{};

	const XMFLOAT3& curPosition{ GetTransform()->GetWorldPosition() };

	m_IsCamUnderWater = m_pWorld->IsPositionWater(curPosition.x, curPosition.y + 0.5f, curPosition.z);

	isUnderWater |= m_pWorld->IsPositionWater(curPosition.x, curPosition.y, curPosition.z);
	isUnderWater |= m_IsCamUnderWater;

	if (prevUnderwater && !isUnderWater)
	{
		constexpr float jumpOutOfWaterForce{ 4.0f };
		m_pMovement->AddVelocity(0.0f, jumpOutOfWaterForce, 0.0f);
	}
	
	m_pMovement->SetUnderWater(isUnderWater);

	XMFLOAT3 feetPos{ GetTransform()->GetWorldPosition() };
	feetPos.y -= 1.0f;
	m_pFeet->GetTransform()->Translate(feetPos);
}