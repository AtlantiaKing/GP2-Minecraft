#include "stdafx.h"
#include "ControllerComponent.h"

ControllerComponent::ControllerComponent(const PxCapsuleControllerDesc& controllerDesc):
	m_ControllerDesc{ controllerDesc }
{
}

void ControllerComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	if(!m_IsInitialized)
	{
		// Complete the capsule description
		const XMFLOAT3& position{ GetTransform()->GetPosition() };
		m_ControllerDesc.position = PxExtendedVec3{ position.x, position.y, position.z };
		m_ControllerDesc.userData = this;

		// Retrieve the controller manager from the PhysxProxy class
		const auto& pControllerManager{ GetGameObject()->GetScene()->GetPhysxProxy()->GetControllerManager() };
		
		// Use the PxControllerManager to create a controller, and store the controller in the appropriate datamember
		m_pController = pControllerManager->createController(m_ControllerDesc);
		ASSERT_NULL_(m_pController);

		// Store a pointer to the ControllerComponent in the userdata of the underlying PxActor
		m_pController->getActor()->userData = this;

		// Set the collision and collisionignore groups
		SetCollisionGroup(static_cast<CollisionGroup>(m_CollisionGroups.word0));
		SetCollisionIgnoreGroup(static_cast<CollisionGroup>(m_CollisionGroups.word1));
	}
}

void ControllerComponent::OnSceneDetach(GameScene*)
{
	if (m_pController)
	{
		m_pController->getActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, true);
	}
}

void ControllerComponent::OnSceneAttach(GameScene*)
{
	if (m_pController)
	{
		m_pController->getActor()->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, false);
	}
}

void ControllerComponent::OnOwnerAttach(GameObject* pOwner)
{
	pOwner->GetTransform()->SetControllerComponent(this);
}

void ControllerComponent::OnOwnerDetach(GameObject* pOwner)
{
	pOwner->GetTransform()->SetControllerComponent(nullptr);
}

void ControllerComponent::ApplyFilterData() const
{
	if (m_pController != nullptr)
	{
		const auto actor = m_pController->getActor();
		const auto numShapes = actor->getNbShapes();
		const auto shapes = new PxShape * [numShapes];

		const auto numPointers = actor->getShapes(shapes, numShapes);
		for (PxU32 i = 0; i < numPointers; i++)
		{
#pragma warning (push)
#pragma warning (disable: 6385)
			const auto shape = shapes[i];
#pragma warning (pop)
			shape->setSimulationFilterData(m_CollisionGroups);
			shape->setQueryFilterData(m_CollisionGroups);
		}
		delete[] shapes;
	}
}

void ControllerComponent::Translate(const XMFLOAT3& pos) const
{
	ASSERT_NULL_(m_pController);
	m_pController->setPosition(PhysxHelper::ToPxExtendedVec3(pos));
}

void ControllerComponent::Move(const XMFLOAT3& displacement, float minDistance)
{
	ASSERT_NULL_(m_pController);
	m_CollisionFlag = m_pController->move(PhysxHelper::ToPxVec3(displacement), minDistance, 0, nullptr, nullptr);
}

XMFLOAT3 ControllerComponent::GetPosition() const
{
	ASSERT_NULL_(m_pController);
	return PhysxHelper::ToXMFLOAT3(m_pController->getPosition());
}

XMFLOAT3 ControllerComponent::GetFootPosition() const
{
	ASSERT_NULL_(m_pController);
	return PhysxHelper::ToXMFLOAT3(m_pController->getFootPosition());
}

void ControllerComponent::SetCollisionGroup(CollisionGroup groups)
{
	m_CollisionGroups.word0 = PxU32(groups);
	ApplyFilterData();
}

void ControllerComponent::SetCollisionIgnoreGroup(CollisionGroup ignoreGroups)
{
	m_CollisionGroups.word1 = PxU32(ignoreGroups);
	ApplyFilterData();
}

void ControllerComponent::SetStepHeight(float stepHeight)
{
	m_pController->setStepOffset(stepHeight);
}
