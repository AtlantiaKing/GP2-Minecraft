#include "stdafx.h"
#include "WorldComponent.h"

#include "Prefabs/ItemEntity.h"
#include "Prefabs/CubePrefab.h"

#include <chrono>

WorldComponent::WorldComponent(const SceneContext& sceneContext)
{
	m_Renderer.LoadEffect(sceneContext);

    m_enablePostDraw = true;

    // Create the cooking interface
    auto& physX{ PxGetPhysics() };
    m_pColliderCooking = PxCreateCooking(PX_PHYSICS_VERSION, physX.getFoundation(), PxCookingParams{ PxTolerancesScale{} });

    // Start a new thread that removes blocks
    m_WorldThread = std::thread{ [this,sceneContext]() { StartWorldThread(sceneContext); } };
    // Start a new thread that has environmental logic
    m_EnvironmentThread = std::thread{ [this,sceneContext]() { StartEnvironmentalChanges(sceneContext); } };
}

WorldComponent::~WorldComponent()
{
    // Release the collider cooking
    m_pColliderCooking->release();

    // Stop the other threads
    m_IsMultithreaded = false;

    // Wait for the other threads to finish
    m_WorldThread.join();
    m_EnvironmentThread.join();
}

void WorldComponent::StartWorldThread(const SceneContext& sceneContext)
{
    // While the thread is alive
    while (m_IsMultithreaded)
    {
        if (m_ShouldRemoveBlock)
        {
            // Remove the block from the right chunk
            m_Generator.RemoveBlock(m_Chunks, m_EditBlockPosition);

            m_Renderer.SetBuffers(m_Chunks, sceneContext, false);
            m_Renderer.SetBuffers(m_Generator.GetWater(), sceneContext, false);

            // Reset ShouldRemoveBlock State
            m_ShouldRemoveBlock = false;

            // Let the main thread know that it should reload vertex buffers and colliders
            m_ShouldSwapWorld = true;
            m_ShouldSwapWater = true;
        }
        else if (m_ShouldPlaceBlock)
        {
            m_ShouldPlaceBlock = false;

            // Remove the block from the right chunk
            m_Generator.PlaceBlock(m_Chunks, m_EditBlockPosition, m_EditBlockType);

            m_Renderer.SetBuffers(m_Chunks, sceneContext, false);

            // Let the main thread know that it should reload vertex buffers and colliders
            m_ShouldSwapWorld = true;
        }
        
        // Don't overdo the CPU with this infinite while loop by sleeping the thread
        std::this_thread::sleep_for(std::chrono::milliseconds{ 10 });
    }
}

void WorldComponent::StartEnvironmentalChanges(const SceneContext& sceneContext)
{
    // Milliseconds per environmental tick
    constexpr int tickTime{ 100 };

    // While the thread is alive
    while (m_IsMultithreaded)
    {
        const auto tickStart{ std::chrono::high_resolution_clock::now() };

        // If the world is loaded, try changing the environment in the generator
        if (m_CanChangeEnvironment && m_Generator.ChangeEnvironment(m_Chunks, m_ChunkCenter))
        {
            m_Renderer.SetBuffers(m_Chunks, sceneContext, false);
            m_Renderer.SetBuffers(m_Generator.GetWater(), sceneContext, false);

            // Let the main thread know that it should reload vertex buffers and colliders
            m_ShouldSwapWorld = true;
            m_ShouldSwapWater = true;
        }

        // Wait until the tick has been completed
        const auto elapsedTickTime{ std::chrono::high_resolution_clock::now() - tickStart };
        const auto sleepTime{ std::chrono::milliseconds(static_cast<int>(tickTime)) - elapsedTickTime };
        std::this_thread::sleep_for(sleepTime);
    }
}

void WorldComponent::PlaceBlock(const XMFLOAT3& hitPos, XMFLOAT3 hitBlockPosition, BlockType block)
{
    const XMFLOAT3 localHitPos{ hitPos.x - hitBlockPosition.x, hitPos.y - hitBlockPosition.y, hitPos.z - hitBlockPosition.z };

    constexpr float blockFaceDistance{ 0.49f };
    if (localHitPos.x >= blockFaceDistance) hitBlockPosition.x += 1.0f;
    else if (localHitPos.x <= -blockFaceDistance) hitBlockPosition.x -= 1.0f;
    else if (localHitPos.y >= blockFaceDistance) hitBlockPosition.y += 1.0f;
    else if (localHitPos.y <= -blockFaceDistance) hitBlockPosition.y -= 1.0f;
    else if (localHitPos.z >= blockFaceDistance) hitBlockPosition.z += 1.0f;
    else if (localHitPos.z <= -blockFaceDistance) hitBlockPosition.z -= 1.0f;

    m_EditBlockPosition = hitBlockPosition;
    m_EditBlockType = block;
    m_ShouldPlaceBlock = true;
}

void WorldComponent::DestroyBlock(const XMFLOAT3& position)
{
    if (m_ShouldRemoveBlock) return;

    m_EditBlockPosition = position;
    m_ShouldRemoveBlock = true;

    // Drop an item on the position of the destroyed block
    const XMINT3 blockPos{ static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.z) };
    Block* pDropBlock{ GetBlockAt(blockPos.x,blockPos.y,blockPos.z)->dropBlock };
    if(pDropBlock) GetScene()->AddChild(new ItemEntity{ pDropBlock->type, position});
}

Block* WorldComponent::GetBlockAt(int x, int y, int z) const
{
    return m_Generator.GetBlockAt(x, y, z, m_Chunks);
}

void WorldComponent::UpdateColliders(const XMFLOAT3& playerPosition)
{
    const int chunkSize{ m_Generator.GetChunkSize() };

    const XMINT2 chunkPos
    {
        playerPosition.x < 0 ? static_cast<int>(playerPosition.x + 1) / chunkSize - 1 : static_cast<int>(playerPosition.x) / chunkSize,
        playerPosition.z < 0 ? static_cast<int>(playerPosition.z + 1) / chunkSize - 1 : static_cast<int>(playerPosition.z) / chunkSize
    };

    if (chunkPos.x != m_ChunkCenter.x || chunkPos.y != m_ChunkCenter.y)
    {
        m_ChunkCenter = chunkPos;

        LoadColliders(true);
    }
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
    // Reload water buffers
    ReloadWater(sceneContext);

    m_CanChangeEnvironment = true;
}

void WorldComponent::Update(const SceneContext& sceneContext)
{
    if (m_ShouldReload)
    {
        ReloadWorld(sceneContext);
        m_ShouldReload = false;
    }
    if (m_ShouldReloadWater)
    {
        ReloadWater(sceneContext);
        m_ShouldReloadWater = false;
    }
    if (m_ShouldSwapWorld) SwapWorld();
    if (m_ShouldSwapWater) SwapWater();
}

void WorldComponent::LoadColliders(bool reloadAll)
{
    auto& physX{ PxGetPhysics() };
    auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 0.0f) };

    for (Chunk& chunk : m_Chunks)
    {
        if (!reloadAll && !chunk.needColliderChange) continue;

        chunk.needColliderChange = false;

        // Remove any chunks that are outside the range
        if (chunk.position.x < (m_ChunkCenter.x - 1) || chunk.position.x > (m_ChunkCenter.x + 1) || chunk.position.y < (m_ChunkCenter.y - 1) || chunk.position.y > (m_ChunkCenter.y + 1))
        {
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

                // Reset the chunk collider data
                chunk.colliderIdx = -1;
                chunk.verticesChanged = true;
            }

            continue;
        }

        LoadChunkCollider(chunk, m_pColliderCooking, physX, pPhysMat);
    }
}

void WorldComponent::LoadChunkCollider(Chunk& chunk, physx::PxCooking* cooking, physx::PxPhysics& physX, physx::PxMaterial* pPhysMat)
{
    // Get all the vertices of the world
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

    chunk.verticesChanged = false;
}

void WorldComponent::ReloadWorld(const SceneContext& sceneContext)
{
    // Set up the vertex buffer
    m_Renderer.SetBuffers(m_Chunks, sceneContext);

    // Create a collider for the world
    LoadColliders();
}

void WorldComponent::ReloadWater(const SceneContext& sceneContext)
{
    // Set up the vertex buffer
    m_Renderer.SetBuffers(m_Generator.GetWater(), sceneContext);
}

void WorldComponent::SwapWorld()
{
    for(Chunk& chunk : m_Chunks)
    {
        if (chunk.pBackVertexBuffer && chunk.pBackVertexBuffer != chunk.pVertexBuffer)
        {
            SafeRelease(chunk.pVertexBuffer);

            chunk.vertexBufferSize = chunk.backVertexBufferSize;
            chunk.pVertexBuffer = chunk.pBackVertexBuffer;
        }
        if (chunk.pBackVertexTransparentBuffer && chunk.pBackVertexTransparentBuffer != chunk.pVertexTransparentBuffer)
        {
            SafeRelease(chunk.pVertexTransparentBuffer);

            chunk.vertexTransparentBufferSize = chunk.backVertexTransparentBufferSize;
            chunk.pVertexTransparentBuffer = chunk.pBackVertexTransparentBuffer;
        }
    }

    m_ShouldSwapWorld = false;

    // Create a collider for the world
    LoadColliders();
}

void WorldComponent::SwapWater()
{
    for (Chunk& chunk : m_Generator.GetWater())
    {
        if (chunk.pBackVertexBuffer && chunk.pBackVertexBuffer != chunk.pVertexBuffer)
        {
            chunk.vertexBufferSize = chunk.backVertexBufferSize;
            chunk.pVertexBuffer = chunk.pBackVertexBuffer;
        }
        if (chunk.pBackVertexTransparentBuffer && chunk.pBackVertexTransparentBuffer != chunk.pVertexTransparentBuffer)
        {
            chunk.vertexTransparentBufferSize = chunk.backVertexTransparentBufferSize;
            chunk.pVertexTransparentBuffer = chunk.pBackVertexTransparentBuffer;
        }
    }

    m_ShouldSwapWater = false;
}

void WorldComponent::Draw(const SceneContext& sceneContext)
{
    m_Renderer.Draw(m_Chunks, sceneContext);
}

void WorldComponent::PostDraw(const SceneContext& sceneContext)
{
    m_Renderer.Draw(m_Generator.GetWater(), sceneContext);
}

void WorldComponent::ShadowMapDraw(const SceneContext& sceneContext)
{
    for (const Chunk& chunk : m_Chunks)
    {
        m_Renderer.DrawShadowMap(chunk, sceneContext);
    }
}
