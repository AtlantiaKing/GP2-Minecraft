#pragma once

class WorldScene;

class PauseMenu final : public GameObject
{
public:
	PauseMenu(WorldScene* pScene);
	~PauseMenu() override = default;

	PauseMenu(const PauseMenu& other) = delete;
	PauseMenu(PauseMenu&& other) noexcept = delete;
	PauseMenu& operator=(const PauseMenu& other) = delete;
	PauseMenu& operator=(PauseMenu&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;
	void OnEnable() override;
	void OnDisable() override;

private:
	void SetPaused(bool paused) const;

	SpriteFont* m_pFont{};

	WorldScene* m_pScene{};
};
