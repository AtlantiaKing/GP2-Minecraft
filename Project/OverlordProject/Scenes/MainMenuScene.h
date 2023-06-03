#pragma once

class WorldComponent;
class PostBlur;

class MainMenuScene final : public GameScene
{
public:
	MainMenuScene() : GameScene(L"Main Menu Scene") {}
	~MainMenuScene() override = default;

	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene&& other) noexcept = delete;
	MainMenuScene& operator=(const MainMenuScene& other) = delete;
	MainMenuScene& operator=(MainMenuScene&& other) noexcept = delete;

protected:
	virtual void Initialize() override;
	virtual void OnSceneActivated() override;
	virtual void Update() override;

private:
	float m_CameraPitch{ 40.0f };
	float m_CameraRotation{};

	SpriteFont* m_pFont{};
	float m_TextSize{};

	GameObject* m_pLoadingBackground{};

	XMFLOAT2 m_pYellowTextPosition{};
	std::wstring m_YellowText{ L"Hello GitHub viewers" };

	WorldComponent* m_pWorld{};

	PostBlur* m_pPostBlur{};
};

