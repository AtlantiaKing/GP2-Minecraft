#pragma once

#include <Misc/World/WorldGenerator.h>
#include <Misc/World/WorldRenderer.h>

#include "Misc/World/Chunk.h"
#include <queue>

class RigidBodyComponent;

class WorldComponent final : public BaseComponent
{
public:
	WorldComponent(const SceneContext& sceneContext);
	virtual ~WorldComponent();

	WorldComponent(const WorldComponent& other) = delete;
	WorldComponent(WorldComponent&& other) noexcept = delete;
	WorldComponent& operator=(const WorldComponent& other) = delete;
	WorldComponent& operator=(WorldComponent&& other) noexcept = delete;

	bool PlaceBlock(const XMFLOAT3& hitPos, XMFLOAT3 hitBlockPosition, BlockType block);
	bool DestroyBlock(const XMFLOAT3& position);
	void UpdateColliders(const XMFLOAT3& playerPosition);
	void SetRenderDistance(int renderDistance);

	Block* GetBlockAt(int x, int y, int z) const;

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext& sceneContext) override;
	virtual void Draw(const SceneContext& sceneContext) override;
	virtual void PostDraw(const SceneContext& sceneContext) override;
	virtual void ShadowMapDraw(const SceneContext& sceneContext) override;
private:
	struct WorldEvent
	{
		bool placeBlock{};
		XMFLOAT3 position{};
		BlockType type{};
	};

	void StartWorldThread(const SceneContext& sceneContext);
	void StartEnvironmentalChanges(const SceneContext& sceneContext);
	void LoadColliders(bool reloadAll = false);
	void LoadChunkCollider(Chunk& chunk, physx::PxCooking* cooking, physx::PxPhysics& physX, physx::PxMaterial* pPhysMat);
	void ReloadWorld(const SceneContext& sceneContext);
	void ReloadWater(const SceneContext& sceneContext);
	void SwapWorld();
	void SwapWater();

	std::thread m_WorldThread{};
	std::thread m_EnvironmentThread{};
	bool m_IsMultithreaded{ true };

	bool m_NeedsWorldReload{};

	bool m_PlaceBlock{};
	bool m_DestroyBlock{};
	XMFLOAT3 m_EditBlock{};
	BlockType m_EditBlockType{};

	std::vector<Chunk> m_Chunks{};
	std::vector<Chunk> m_WaterChunks{};

	WorldGenerator m_Generator{};
	WorldRenderer m_Renderer{};

	RigidBodyComponent* m_pRb{};
	PxCooking* m_pColliderCooking{};

	XMINT2 m_ChunkCenter{};

	bool m_CanChangeEnvironment{};
};

