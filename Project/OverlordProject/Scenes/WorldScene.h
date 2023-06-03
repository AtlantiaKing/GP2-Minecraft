#pragma once

class WorldComponent;
class PostUnderWater;
class Player;

class WorldScene final : public GameScene
{
public:
	WorldScene() : GameScene(L"World Scene") {}
	~WorldScene() override = default;

	WorldScene(const WorldScene& other) = delete;
	WorldScene(WorldScene&& other) noexcept = delete;
	WorldScene& operator=(const WorldScene& other) = delete;
	WorldScene& operator=(WorldScene&& other) noexcept = delete;

	void Pause(bool isPaused);

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	virtual void OnSceneActivated() override;

private:
	void CreateWorld();

	Player* m_pPlayer{};
	GameObject* m_pSelection{};
	WorldComponent* m_pWorld{};

	PostUnderWater* m_pUnderwater{};

	bool m_IsPaused{};
	std::vector<GameObject*> m_pGameObjectToPause{};
	std::vector<GameObject*> m_pGameObjectToHideOnPause{};
};



