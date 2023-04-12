#pragma once

#include <Misc/World/WorldGenerator.h>
#include <Misc/World/WorldRenderer.h>

class RigidBodyComponent;

class WorldComponent final : public BaseComponent
{
public:
	WorldComponent(const SceneContext& sceneContext);
	virtual ~WorldComponent() = default;

	WorldComponent(const WorldComponent& other) = delete;
	WorldComponent(WorldComponent&& other) noexcept = delete;
	WorldComponent& operator=(const WorldComponent& other) = delete;
	WorldComponent& operator=(WorldComponent&& other) noexcept = delete;

	void DestroyBlock(const XMFLOAT3& position, const SceneContext& sceneContext);
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Draw(const SceneContext& sceneContext) override;

private:
	void LoadCollider() const;

	WorldGenerator m_Generator{};
	WorldRenderer m_Renderer{};

	RigidBodyComponent* m_pRb{};
};

