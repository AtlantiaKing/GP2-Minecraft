#pragma once

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
	void Initialize() override;
	void Update() override;

private:
	float m_CameraPitch{ 40.0f };
	float m_CameraRotation{};

	PostBlur* m_pPostBlur{};
};

