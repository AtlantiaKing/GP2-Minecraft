#pragma once

class WorldScene final : public GameScene
{
public:
	WorldScene() : GameScene(L"World Scene") {}
	~WorldScene() override = default;

	WorldScene(const WorldScene& other) = delete;
	WorldScene(WorldScene&& other) noexcept = delete;
	WorldScene& operator=(const WorldScene& other) = delete;
	WorldScene& operator=(WorldScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void OnGUI() override;

private:
};



