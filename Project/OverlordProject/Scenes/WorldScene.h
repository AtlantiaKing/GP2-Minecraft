#pragma once

class WorldComponent;
class BlockBreakParticle;
class Player;
class PostUnderWater;

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
	void OnPlayerDeath();

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;
	virtual void OnSceneActivated() override;
	virtual void OnSceneDeactivated() override;

private:
	void CreateWorld();
	void CreateUI();
	void CreatePlayerFeedback();
	void CreateHUD();
	void CreateSkyBox();

	void PlaySong();

	Player* m_pPlayer{};
	BlockBreakParticle* m_pBlockBreakParticle{};
	GameObject* m_pSelection{};
	WorldComponent* m_pWorld{};

	PostUnderWater* m_pUnderwater{};

	bool m_IsPaused{};
	std::vector<GameObject*> m_pGameObjectToHideOnPause{};

	const float m_TimeUntilSong{ 5.0f * 60.0f };
	float m_CurSongTimer{};

	const int m_NrSounds{ 3 };
	std::vector<FMOD::Sound*> m_pSounds{};
	FMOD::Channel* m_pChannel{};
};



