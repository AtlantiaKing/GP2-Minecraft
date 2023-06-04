#include "stdafx.h"
#include "SheepPrefab.h"

#include "Components/LivingEntities/Sheep.h"
#include "Components/Health.h"

#include "Materials/Shadow/DiffuseMaterial_Shadow_Skinned.h"

void SheepPrefab::Initialize(const SceneContext&)
{
	GetTransform()->Scale(0.02f);

	DiffuseMaterial_Shadow_Skinned* pSheepMaterial{ MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow_Skinned>() };
	pSheepMaterial->SetDiffuseTexture(L"Textures/Sheep/Sheep.dds");
	AddComponent(new ModelComponent{ L"Meshes/Sheep.ovm", true })->SetMaterial(pSheepMaterial);

	AddComponent(new Health{ 4 });

	const XMFLOAT3 hitboxHalfDimensions{ 0.25f,0.5f,0.25f };

	auto& physX{ PxGetPhysics() };
	auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };
	RigidBodyComponent* pSheepRb{ AddComponent(new RigidBodyComponent{}) };
	pSheepRb->AddCollider(PxBoxGeometry{ hitboxHalfDimensions.x,hitboxHalfDimensions.y, hitboxHalfDimensions.z }, *pPhysMat, false, PxTransform{ 0.0f, hitboxHalfDimensions.y, 0.0f });
	pSheepRb->SetConstraint(RigidBodyConstraint::AllRot, false);
	pSheepRb->SetCollisionGroup(CollisionGroup::DefaultCollision | CollisionGroup::LivingEntity);

	AddComponent(new Sheep{ hitboxHalfDimensions });
}
