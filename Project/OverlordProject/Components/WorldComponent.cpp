#include "stdafx.h"
#include "WorldComponent.h"

#include "Prefabs/ItemEntity.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/Particles/BlockDestroyParticle.h"

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

    for (Chunk& chunk : m_Chunks) chunk.DeleteChunk();
    for (Chunk& chunk : m_WaterChunks) chunk.DeleteChunk();
}

void WorldComponent::StartWorldThread(const SceneContext& sceneContext)
{
    const int environmentTick{ 250 }; // in milliseconds
    const int worldTick{ 5 }; // in milliseconds

    // Current time until a environment change
    int curEnvironmentTick{};

    // While the thread is alive
    while (m_IsMultithreaded)
    {
        // Get the current time
        const auto frameStart{ std::chrono::high_resolution_clock::now() };

        if (!m_NeedsWorldReload) // don't change the world while the main thread is updating
        {
            if (m_DestroyBlock)
            {
                // Remove the block from the right chunk
                m_Generator.RemoveBlock(m_EditBlock, sceneContext, &m_Renderer);

                // Let the main thread now to get the latest worlddata
                m_NeedsWorldReload = true;

                // Disable destroy block flag
                m_DestroyBlock = false;
            }
            else if (m_PlaceBlock)
            {
                // Place the block in the right chunk
                m_Generator.PlaceBlock(m_EditBlock, m_EditBlockType, sceneContext, &m_Renderer);

                // Let the main thread now to get the latest worlddata
                m_NeedsWorldReload = true;

                // Disable place block flag
                m_PlaceBlock = false;
            }
            else
            {
                bool needsReload{};

                // Try loading a chunk
                needsReload = m_Generator.LoadChunk(m_ChunkCenter, sceneContext, &m_Renderer);

                // If the world isn't updated yet, try updating the world
                if (!needsReload && curEnvironmentTick >= environmentTick)
                {
                    curEnvironmentTick = 0;
                    needsReload = m_Generator.ChangeEnvironment(m_ChunkCenter, sceneContext, &m_Renderer);
                }

                // Let the main thread now whether or not to get the latest worlddata
                m_NeedsWorldReload = needsReload;
            }
        }

        // Sleep so that the total frame time becomes the desired frame time
        const auto frameTime{ std::chrono::high_resolution_clock::now() - frameStart };
        const auto sleepTime{ std::chrono::milliseconds(worldTick) - frameTime };
        std::this_thread::sleep_for(sleepTime);

        // Increment environment timer
        const auto endOfFrameTime{ std::chrono::high_resolution_clock::now() - frameStart };
        curEnvironmentTick += static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(endOfFrameTime).count());
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

bool WorldComponent::PlaceBlock(const XMFLOAT3& hitNormal, XMFLOAT3 hitBlockPosition, BlockType block)
{
    if (m_NeedsWorldReload || m_PlaceBlock) return false;

    hitBlockPosition.x += hitNormal.x;
    hitBlockPosition.y += hitNormal.y;
    hitBlockPosition.z += hitNormal.z;

    m_EditBlock = hitBlockPosition;
    m_EditBlockType = block;
    m_PlaceBlock = true;

    return true;
}

bool WorldComponent::DestroyBlock(const XMFLOAT3& position)
{
    if (m_NeedsWorldReload || m_DestroyBlock) return false;

    // Drop an item on the position of the destroyed block
    const XMINT3 blockPos{ static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.z) };
    Block* pDropBlock{ GetBlockAt(blockPos.x,blockPos.y,blockPos.z)->dropBlock };
    if (pDropBlock)
    {
        GetScene()->AddChild(new ItemEntity{ pDropBlock->type, position });
        GetScene()->AddChild(new BlockDestroyParticle{ pDropBlock->type })->GetTransform()->Translate(position);
    }

    Block* pBlockUp{ GetBlockAt(blockPos.x,blockPos.y+1,blockPos.z) };
    if (pBlockUp && pBlockUp->mesh == BlockMesh::CROSS)
        GetScene()->AddChild(new BlockDestroyParticle{ pBlockUp->type })->GetTransform()->Translate(position.x, position.y + 1.0f, position.z);

    m_EditBlock = position;
    m_DestroyBlock = true;

    return true;
}

Block* WorldComponent::GetBlockAt(int x, int y, int z) const
{
    return m_Generator.GetBlockAt(x, y, z);
}

bool WorldComponent::IsPositionWater(float worldX, float worldY, float worldZ) const
{
    const int x{ worldX > 0.0f ? static_cast<int>(worldX + 0.5f) : static_cast<int>(worldX + 0.5f) - 1 };
    const int y{ worldY > 0.0f ? static_cast<int>(worldY + 0.5f) : static_cast<int>(worldY + 0.5f) - 1 };
    const int z{ worldZ > 0.0f ? static_cast<int>(worldZ + 0.5f) : static_cast<int>(worldZ + 0.5f) - 1 };
    return GetBlockAt(x,y,z, m_WaterChunks) == BlockType::WATER;
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

void WorldComponent::LoadStartChunk(const SceneContext& sceneContext)
{
    m_NeedsWorldReload = true;
    m_Generator.LoadChunkMainThread(0, 0, sceneContext, &m_Renderer);

#ifndef _DEBUG
    for (int x{ -1 }; x <= 1; ++x)
    {
        for (int y{ -1 }; y <= 1; ++y)
        {
            if (x == 0 && y == 0) continue;

            m_Generator.LoadChunkMainThread(x, y, sceneContext, &m_Renderer);
        }
    }
#endif

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
        const auto& genChunks{ m_Generator.GetChunks() };
        const auto& genWaterChunks{ m_Generator.GetWater() };

        for (int i{ static_cast<int>(m_Chunks.size() - 1) }; i >= 0; --i)
        {
            auto& curChunk{ m_Chunks[i] };
            auto chunkIt{ std::find_if(begin(genChunks), end(genChunks), [&](const Chunk& chunk) { return chunk.position.x == curChunk.position.x && chunk.position.y == curChunk.position.y; }) };
            if (chunkIt != genChunks.end()) continue;
            
            curChunk.DeleteChunk();
            m_Chunks[i] = m_Chunks[m_Chunks.size() - 1];
            m_Chunks.pop_back();
        }
        for (int i{ static_cast<int>(m_WaterChunks.size() - 1) }; i >= 0; --i)
        {
            auto& curChunk{ m_WaterChunks[i] };
            auto chunkIt{ std::find_if(begin(genWaterChunks), end(genWaterChunks), [&](const Chunk& chunk) { return chunk.position.x == curChunk.position.x && chunk.position.y == curChunk.position.y; }) };
            if (chunkIt != genWaterChunks.end()) continue;

            curChunk.DeleteChunk();
            m_WaterChunks[i] = m_WaterChunks[m_WaterChunks.size() - 1];
            m_WaterChunks.pop_back();
        }

        for (const auto& genChunk : genChunks)
        {
            const auto chunkIt{ std::find_if(begin(m_Chunks), end(m_Chunks), 
                [&](const Chunk& chunk) { return chunk.position.x == genChunk.position.x && chunk.position.y == genChunk.position.y; }) };

            if(chunkIt != end(m_Chunks))
            {
                auto& chunk{ *chunkIt };

                chunk.position = genChunk.position;

                if (chunk.pVertexBuffer != genChunk.pVertexBuffer)
                {
                    SafeRelease(chunk.pVertexBuffer);

                    chunk.vertices = genChunk.vertices;
                    chunk.pVertexBuffer = genChunk.pVertexBuffer;
                    chunk.vertexBufferSize = genChunk.vertexBufferSize;
                    chunk.needColliderChange = true;
                }
                if (chunk.pVertexTransparentBuffer != genChunk.pVertexTransparentBuffer)
                {
                    SafeRelease(chunk.pVertexTransparentBuffer);

                    chunk.pVertexTransparentBuffer = genChunk.pVertexTransparentBuffer;
                    chunk.vertexTransparentBufferSize = genChunk.vertexTransparentBufferSize;
                }
            }
            else
            {
                Chunk chunk{};

                chunk.position = genChunk.position;

                chunk.vertices = genChunk.vertices;
                chunk.pVertexBuffer = genChunk.pVertexBuffer;
                chunk.vertexBufferSize = genChunk.vertexBufferSize;
                chunk.needColliderChange = true;

                chunk.pVertexTransparentBuffer = genChunk.pVertexTransparentBuffer;
                chunk.vertexTransparentBufferSize = genChunk.vertexTransparentBufferSize;

                m_Chunks.push_back(chunk);
            }
        }

        for (const auto& genChunk : genWaterChunks)
        {
            const auto chunkIt{ std::find_if(begin(m_WaterChunks), end(m_WaterChunks), 
                [&](const Chunk& chunk) { return chunk.position.x == genChunk.position.x && chunk.position.y == genChunk.position.y; }) };

            if (chunkIt != end(m_WaterChunks))
            {
                auto& chunk{ *chunkIt };

                chunk.position = genChunk.position;

                if (chunk.pVertexBuffer != genChunk.pVertexBuffer)
                {
                    SafeRelease(chunk.pVertexBuffer);

                    chunk.pVertexBuffer = genChunk.pVertexBuffer;
                    chunk.vertexBufferSize = genChunk.vertexBufferSize;
                }

                if (chunk.pVertexTransparentBuffer != genChunk.pVertexTransparentBuffer)
                {
                    SafeRelease(chunk.pVertexTransparentBuffer);

                    chunk.blocks = genChunk.blocks;
                    chunk.pVertexTransparentBuffer = genChunk.pVertexTransparentBuffer;
                    chunk.vertexTransparentBufferSize = genChunk.vertexTransparentBufferSize;
                }
            }
            else
            {
                Chunk chunk{};

                chunk.position = genChunk.position;
                chunk.blocks = genChunk.blocks;

                chunk.pVertexBuffer = genChunk.pVertexBuffer;
                chunk.vertexBufferSize = genChunk.vertexBufferSize;

                chunk.pVertexTransparentBuffer = genChunk.pVertexTransparentBuffer;
                chunk.vertexTransparentBufferSize = genChunk.vertexTransparentBufferSize;

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

BlockType WorldComponent::GetBlockAt(int x, int y, int z, const std::vector<Chunk>& chunks) const
{
    const int chunkSize{ m_Generator.GetChunkSize() };

    const XMINT2 chunkPos
    {
        x < 0 ? (static_cast<int>(x) + 1) / chunkSize - 1 : static_cast<int>(x) / chunkSize,
        z < 0 ? (static_cast<int>(z) + 1) / chunkSize - 1 : static_cast<int>(z) / chunkSize
    };

    auto it{ std::find_if(begin(chunks), end(chunks), [&](const Chunk& chunk)
        {
            return chunk.position.x == chunkPos.x && chunk.position.y == chunkPos.y;
        }) };
    if (it == chunks.end()) return BlockType::AIR;

    if (it->blocks.empty()) return BlockType::AIR;

    const XMINT3 lookUpPos{ static_cast<int>(x) - chunkPos.x * chunkSize, static_cast<int>(y), static_cast<int>(z) - chunkPos.y * chunkSize };

    if (lookUpPos.x < 0 || lookUpPos.x >= chunkSize
        || lookUpPos.z < 0 || lookUpPos.z >= chunkSize
        || lookUpPos.y < 0 || lookUpPos.y >= m_Generator.GetWorldHeight()) return BlockType::AIR;

    const int blockIdx{ lookUpPos.x + lookUpPos.z * chunkSize + lookUpPos.y * chunkSize * chunkSize };

    return *(it->blocks.data() + blockIdx);
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
