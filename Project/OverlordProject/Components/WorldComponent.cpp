#include "stdafx.h"
#include "WorldComponent.h"

WorldComponent::WorldComponent(const SceneContext& sceneContext)
{
	m_Renderer.LoadEffect(sceneContext);
}

WorldComponent::~WorldComponent()
{
    for (const Chunk& chunk : m_Chunks)
    {
        for (Block* pBlock : chunk.pBlocks)
        {
            delete pBlock;
        }
    }
}

void WorldComponent::DestroyBlock(const XMFLOAT3& position, const SceneContext& sceneContext)
{
    // Remove the block from the right chunk
    m_Generator.RemoveBlock(m_Chunks, position);

    // Reload buffers and colliders
    ReloadWorld(sceneContext);
}

void WorldComponent::Initialize(const SceneContext& sceneContext)
{
    // Add a rigidbody component to the world gameobject
    m_pRb = GetGameObject()->AddComponent(new RigidBodyComponent{true});
    m_pRb->SetCollisionGroup(CollisionGroup::DefaultCollision | CollisionGroup::World);

    // Load the chunks
    m_Generator.LoadWorld(m_Chunks);

    // Reload buffers and colliders
    ReloadWorld(sceneContext);

    m_Renderer.SetBuffer(m_Generator.GetWater(), sceneContext);
}

void WorldComponent::LoadColliders()
{
    auto& physX{ PxGetPhysics() };
    auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };

    // Create the cooking interface
    PxCooking* cooking = PxCreateCooking(PX_PHYSICS_VERSION, physX.getFoundation(), PxCookingParams{ PxTolerancesScale{} });

    for (Chunk& chunk : m_Chunks)
    {
        // TODO: Replace with 3x3 chunks around the player
        if (chunk.position.x < -1 || chunk.position.x > 1 || chunk.position.y < -1 || chunk.position.y > 1) continue;

        if (!chunk.verticesChanged) continue;

        // Get all the vertices of the world (these do not include water)
        std::vector<XMFLOAT3> vertices{ m_Generator.GetPositions(chunk) };

        // Create a list of indices (starts at 0 and ends at nrVertices)
        std::vector<PxU32> indices{};
        indices.reserve(vertices.size());
        for (PxU32 i{}; i < static_cast<PxU32>(vertices.size()); ++i)
        {
            indices.emplace_back(i);
        }

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

        // Remove the previous collider if it exists
        if (chunk.colliderIdx >= 0)
        {
            // Remove the collider
            m_pRb->RemoveCollider(m_pRb->GetCollider(chunk.colliderIdx));

            // Decrement the colliderIdx of every chunk if its higher then the current colliderIdx
            for (Chunk& otherChunk : m_Chunks)
            {
                if (otherChunk.colliderIdx > chunk.colliderIdx) --otherChunk.colliderIdx;
            }
        }

        // Add the collider
        chunk.colliderIdx = m_pRb->AddCollider(geometry, *pPhysMat);
    }

    // Cleanup
    cooking->release();
}

void WorldComponent::ReloadWorld(const SceneContext& sceneContext)
{
    // Set up the vertex buffers
    m_Renderer.SetBuffers(m_Chunks, sceneContext);

    // Create a collider for the world
    LoadColliders();

    // Reset the vertices changed flags
    for (Chunk& chunk : m_Chunks)
    {
        chunk.verticesChanged = false;
    }
}

void WorldComponent::Draw(const SceneContext& sceneContext)
{
    m_Renderer.Draw(m_Chunks, sceneContext);
    m_Renderer.Draw(m_Generator.GetWater(), sceneContext);
}
