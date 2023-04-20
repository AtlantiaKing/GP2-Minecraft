#pragma once

#include <Misc/World/WorldGenerator.h>
#include <Misc/World/WorldRenderer.h>

#include "Misc/World/Chunk.h"

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

	void PlaceBlock(const XMFLOAT3& hitPos, XMFLOAT3 hitBlockPosition, BlockType block);
	void DestroyBlock(const XMFLOAT3& position);
	void UpdateColliders(const XMFLOAT3& playerPosition);
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext& sceneContext) override;
	virtual void Draw(const SceneContext& sceneContext) override;

private:
	void StartWorldThread();
	void StartEnvironmentalChanges();
	void LoadColliders();
	void LoadChunkCollider(Chunk& chunk, physx::PxCooking* cooking, physx::PxPhysics& physX, physx::PxMaterial* pPhysMat);
	void ReloadWorld(const SceneContext& sceneContext);
	void ReloadWater(const SceneContext& sceneContext);

	std::thread m_WorldThread{};
	std::thread m_EnvironmentThread{};
	bool m_IsMultithreaded{ true };

	std::vector<Chunk> m_Chunks{};

	WorldGenerator m_Generator{};
	WorldRenderer m_Renderer{};

	RigidBodyComponent* m_pRb{};
	PxCooking* m_pColliderCooking{};

	XMINT2 m_ChunkCenter{};
	bool m_ShouldRemoveBlock{};
	bool m_ShouldPlaceBlock{};
	XMFLOAT3 m_EditBlockPosition{};
	BlockType m_EditBlockType{};
	bool m_ShouldReload{};
	bool m_ShouldReloadWater{};
	bool m_CanChangeEnvironment{};
};

