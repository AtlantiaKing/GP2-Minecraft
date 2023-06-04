#pragma once

class WorldScene;

class DeathScreen final : public GameObject
{
public:
	DeathScreen() = default;
	~DeathScreen() override = default;

	DeathScreen(const DeathScreen& other) = delete;
	DeathScreen(DeathScreen&& other) noexcept = delete;
	DeathScreen& operator=(const DeathScreen& other) = delete;
	DeathScreen& operator=(DeathScreen&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;
	void OnEnable() override;

private:
	SpriteFont* m_pFont{};
};
