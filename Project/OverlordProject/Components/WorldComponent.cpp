#include "stdafx.h"
#include "WorldComponent.h"

#include "Prefabs/ItemEntity.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/Particles/BlockBreakParticle.h"

#include <chrono>

WorldComponent::WorldComponent(const SceneContext& sceneContext)
{
	m_Renderer.LoadEffect(sceneContext);

    m_enablePostDraw = true;

    // Create the cooking interface
    auto& physX{ PxGetPhysics() };
    m_pColliderCooking = PxCreateCooking(PX_PHYSICS_VERSION, physX.getFoundation(), PxCookingParams{ PxTolerancesScale{} });
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
        if (m_DestroyBlock)
        {
            // Remove the block from the right chunk
            m_Generator.RemoveBlock(m_EditBlock, sceneContext, &m_Renderer);

            m_NeedsWorldReload = true;

            m_DestroyBlock = false;
        }
        else if (m_PlaceBlock)
        {
            // Remove the block from the right chunk
            m_Generator.PlaceBlock(m_EditBlock, m_EditBlockType, sceneContext, &m_Renderer);

            // Let the main thread know that it should reload vertex buffers and colliders
            m_NeedsWorldReload = true;

            m_PlaceBlock = false;
        }
        else if (!m_NeedsWorldReload)
        {
            m_NeedsWorldReload = m_Generator.LoadChunk(m_ChunkCenter, sceneContext, &m_Renderer);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds{ 1 });
    }
}

void WorldComponent::StartEnvironmentalChanges(const SceneContext&)
{
    // Milliseconds per environmental tick
    constexpr int tickTime{ 100 };

    // While the thread is alive
    while (m_IsMultithreaded)
    {
        const auto tickStart{ std::chrono::high_resolution_clock::now() };

        const auto elapsedTickTime{ std::chrono::high_resolution_clock::now() - tickStart };
        const auto sleepTime{ std::chrono::milliseconds(static_cast<int>(tickTime)) - elapsedTickTime };
        std::this_thread::sleep_for(sleepTime);
    }
}

bool WorldComponent::PlaceBlock(const XMFLOAT3& hitPos, XMFLOAT3 hitBlockPosition, BlockType block)
{
    if (m_NeedsWorldReload || m_PlaceBlock) return false;

    const XMFLOAT3 localHitPos{ hitPos.x - hitBlockPosition.x, hitPos.y - hitBlockPosition.y, hitPos.z - hitBlockPosition.z };

    constexpr float blockFaceDistance{ 0.49f };
    if (localHitPos.x >= blockFaceDistance) hitBlockPosition.x += 1.0f;
    else if (localHitPos.x <= -blockFaceDistance) hitBlockPosition.x -= 1.0f;
    else if (localHitPos.y >= blockFaceDistance) hitBlockPosition.y += 1.0f;
    else if (localHitPos.y <= -blockFaceDistance) hitBlockPosition.y -= 1.0f;
    else if (localHitPos.z >= blockFaceDistance) hitBlockPosition.z += 1.0f;
    else if (localHitPos.z <= -blockFaceDistance) hitBlockPosition.z -= 1.0f;

    m_EditBlock = hitBlockPosition;
    m_EditBlockType = block;
    m_PlaceBlock = true;

    return true;
}

bool WorldComponent::DestroyBlock(const XMFLOAT3& position)
{
    if (m_NeedsWorldReload || m_DestroyBlock) return false;

    m_EditBlock = position;
    m_DestroyBlock = true;

    // Drop an item on the position of the destroyed block
    const XMINT3 blockPos{ static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.z) };
    Block* pDropBlock{ GetBlockAt(blockPos.x,blockPos.y,blockPos.z)->dropBlock };
    if (pDropBlock)
    {
        GetScene()->AddChild(new ItemEntity{ pDropBlock->type, position });
        GetScene()->AddChild(new BlockBreakParticle{ pDropBlock->type })->GetTransform()->Translate(position);
    }

    Block* pBlockUp{ GetBlockAt(blockPos.x,blockPos.y+1,blockPos.z) };
    if (pBlockUp && pBlockUp->mesh == BlockMesh::CROSS)
        GetScene()->AddChild(new BlockBreakParticle{ pBlockUp->type })->GetTransform()->Translate(position.x, position.y + 1.0f, position.z);

    return true;
}

Block* WorldComponent::GetBlockAt(int x, int y, int z) const
{
    return m_Generator.GetBlockAt(x, y, z);
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

void WorldComponent::SetRenderDistance(int renderDistance)
{
    m_Generator.SetRenderDistance(renderDistance);
}

void WorldComponent::Initialize(const SceneContext& sceneContext)
{
    // Add a rigidbody component to the world gameobject
    m_pRb = GetGameObject()->AddComponent(new RigidBodyComponent{true});
    m_pRb->SetCollisionGroup(CollisionGroup::DefaultCollision | CollisionGroup::World);

    m_CanChangeEnvironment = true;

    // Start a new thread that removes blocks
    m_WorldThread = std::thread{ [this,sceneContext]() { StartWorldThread(sceneContext); } };
    // Start a new thread that has environmental logic
    m_EnvironmentThread = std::thread{ [this,sceneContext]() { StartEnvironmentalChanges(sceneContext); } };
}

void WorldComponent::Update(const SceneContext&)
{
    if (m_NeedsWorldReload)
    {
        auto& genChunks{ m_Generator.GetChunks() };
        auto& genWaterChunks{ m_Generator.GetWater() };

        for (int i{}; i < genChunks.size(); ++i)
        {
            auto& genChunk{ genChunks[i] };

            auto chunkIt{ std::find_if(begin(m_Chunks), end(m_Chunks), [&](const Chunk& chunk) { return chunk.position.x == genChunk.position.x && chunk.position.y == genChunk.position.y; }) };

            if(chunkIt != end(m_Chunks))
            {
                auto& chunk{ *chunkIt };

                chunk.position = genChunk.position;
                //chunk.pBlocks = genChunk.pBlocks;
                if (chunk.pVertexBuffer != genChunk.pVertexBuffer)
                {
                    chunk.vertices = genChunk.vertices;
                    if (chunk.pVertexBuffer) SafeRelease(chunk.pVertexBuffer);
                    chunk.pVertexBuffer = genChunk.pVertexBuffer;
                    chunk.vertexBufferSize = genChunk.vertexBufferSize;
                    chunk.needColliderChange = true;
                }
                if (chunk.pVertexTransparentBuffer != genChunk.pVertexTransparentBuffer)
                {
                    if (chunk.pVertexTransparentBuffer) SafeRelease(chunk.pVertexTransparentBuffer);
                    chunk.pVertexTransparentBuffer = genChunk.pVertexTransparentBuffer;
                    chunk.vertexTransparentBufferSize = genChunk.vertexTransparentBufferSize;
                }
            }
            else
            {
                Chunk chunk{};

                chunk.position = genChunk.position;
                //chunk.pBlocks = genChunk.pBlocks;
                if (chunk.pVertexBuffer != genChunk.pVertexBuffer)
                {
                    chunk.vertices = genChunk.vertices;
                    if (chunk.pVertexBuffer) SafeRelease(chunk.pVertexBuffer);
                    chunk.pVertexBuffer = genChunk.pVertexBuffer;
                    chunk.vertexBufferSize = genChunk.vertexBufferSize;
                    chunk.needColliderChange = true;
                }
                if (chunk.pVertexTransparentBuffer != genChunk.pVertexTransparentBuffer)
                {
                    if (chunk.pVertexTransparentBuffer) SafeRelease(chunk.pVertexTransparentBuffer);
                    chunk.pVertexTransparentBuffer = genChunk.pVertexTransparentBuffer;
                    chunk.vertexTransparentBufferSize = genChunk.vertexTransparentBufferSize;
                }
                m_Chunks.push_back(chunk);
            }
        }

        for (int i{}; i < genWaterChunks.size(); ++i)
        {
            if (m_WaterChunks.size() > i)
            {
                auto& chunk{ m_WaterChunks[i] };
                auto& genChunk{ genWaterChunks[i] };
                chunk.position = genChunk.position;
                if (chunk.pVertexBuffer != genChunk.pVertexBuffer)
                {
                    //if (chunk.pVertexBuffer) SafeRelease(chunk.pVertexBuffer);
                    chunk.pVertexBuffer = genChunk.pVertexBuffer;
                    chunk.vertexBufferSize = genChunk.vertexBufferSize;
                }
                if (chunk.pVertexTransparentBuffer != genChunk.pVertexTransparentBuffer)
                {
                    //if (chunk.pVertexTransparentBuffer) SafeRelease(chunk.pVertexTransparentBuffer);
                    chunk.pVertexTransparentBuffer = genChunk.pVertexTransparentBuffer;
                    chunk.vertexTransparentBufferSize = genChunk.vertexTransparentBufferSize;
                }
            }
            else
            {
                Chunk chunk{};
                auto& genChunk{ genWaterChunks[i] };
                chunk.position = genChunk.position;
                if (chunk.pVertexBuffer != genChunk.pVertexBuffer)
                {
                    //if (chunk.pVertexBuffer) SafeRelease(chunk.pVertexBuffer);
                    chunk.pVertexBuffer = genChunk.pVertexBuffer;
                    chunk.vertexBufferSize = genChunk.vertexBufferSize;
                }
                if (chunk.pVertexTransparentBuffer != genChunk.pVertexTransparentBuffer)
                {
                    //if (chunk.pVertexTransparentBuffer) SafeRelease(chunk.pVertexTransparentBuffer);
                    chunk.pVertexTransparentBuffer = genChunk.pVertexTransparentBuffer;
                    chunk.vertexTransparentBufferSize = genChunk.vertexTransparentBufferSize;
                }
                m_WaterChunks.push_back(chunk);
            }
        }

        LoadColliders();

        m_NeedsWorldReload = false;
    }
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

void WorldComponent::Draw(const SceneContext& sceneContext)
{
    m_Renderer.Draw(m_Chunks, sceneContext);
}

void WorldComponent::PostDraw(const SceneContext& sceneContext)
{
    m_Renderer.Draw(m_WaterChunks, sceneContext);
}

void WorldComponent::ShadowMapDraw(const SceneContext& sceneContext)
{
    for (const Chunk& chunk : m_Chunks)
    {
        m_Renderer.DrawShadowMap(chunk, sceneContext);
    }
}
