#include "stdafx.h"
#include "WorldComponent.h"

WorldComponent::WorldComponent(const SceneContext& sceneContext)
{
	m_Renderer.LoadEffect(sceneContext);
}

void WorldComponent::DestroyBlock(const XMFLOAT3& position, const SceneContext& sceneContext)
{
    // Load the world and set up the vertex buffer
    m_Renderer.SetBuffer(m_Generator.RemoveBlock(position), sceneContext);

    // Create a collider for the world
    LoadCollider();
}

void WorldComponent::Initialize(const SceneContext& sceneContext)
{
    // Add a rigidbody component to the world gameobject
    m_pRb = GetGameObject()->AddComponent(new RigidBodyComponent{true});
    m_pRb->SetCollisionGroup(CollisionGroup::DefaultCollision | CollisionGroup::World);

    // Load the world and set up the vertex buffer
	m_Renderer.SetBuffer(m_Generator.LoadWorld(), sceneContext);

    // Create a collider for the world
    LoadCollider();
}

void WorldComponent::LoadCollider() const
{
    // Get all the vertices of the world (these do not include water)
    std::vector<XMFLOAT3> vertices{ m_Generator.GetPositions() };

    // Create a list of indices (starts at 0 and ends at nrVertices)
    std::vector<PxU32> indices{};
    indices.reserve(vertices.size());
    for (PxU32 i{}; i < static_cast<PxU32>(vertices.size()); ++i)
    {
        indices.emplace_back(i);
    }

    auto& physX{ PxGetPhysics() };
    auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };

    // Create the cooking interface
    PxCooking* cooking = PxCreateCooking(PX_PHYSICS_VERSION, physX.getFoundation(), PxCookingParams{ PxTolerancesScale{} });

    // Create the triangle mesh desc
    PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = static_cast<PxU32>(vertices.size());
    meshDesc.points.stride = sizeof(PxVec3);
    meshDesc.points.data = vertices.data();
    meshDesc.triangles.count = static_cast<PxU32>(vertices.size()) / 3;
    meshDesc.triangles.stride = 3 * sizeof(PxU32);
    meshDesc.triangles.data = indices.data();

    // Cook the mesh
    PxDefaultMemoryOutputStream writeBuffer;
    cooking->cookTriangleMesh(meshDesc, writeBuffer);

    // Create the triangle mesh
    PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
    PxTriangleMesh* triangleMesh = physX.createTriangleMesh(readBuffer);

    // Create the geometry
    PxTriangleMeshGeometry geometry{ triangleMesh };

    // Remove any previous colliders
    m_pRb->RemoveColliders();

    // Add the collider
    m_pRb->AddCollider(geometry, *pPhysMat);

    // Cleanup
    cooking->release();
}

void WorldComponent::Draw(const SceneContext& sceneContext)
{
	m_Renderer.Draw(sceneContext);
}
