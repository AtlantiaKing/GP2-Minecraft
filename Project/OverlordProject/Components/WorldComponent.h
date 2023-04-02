#pragma once

#include <Misc/World/WorldGenerator.h>
#include <Misc/World/WorldRenderer.h>

class WorldComponent final : public BaseComponent
{
public:
	WorldComponent(const SceneContext& sceneContext);
	virtual ~WorldComponent() = default;

	WorldComponent(const WorldComponent& other) = delete;
	WorldComponent(WorldComponent&& other) noexcept = delete;
	WorldComponent& operator=(const WorldComponent& other) = delete;
	WorldComponent& operator=(WorldComponent&& other) noexcept = delete;

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Draw(const SceneContext& sceneContext) override;

private:
	WorldGenerator m_Generator{};
	WorldRenderer m_Renderer{};
};
