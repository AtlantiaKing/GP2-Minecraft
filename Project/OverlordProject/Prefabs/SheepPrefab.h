#pragma once
class SheepPrefab : public GameObject
{
public:
	SheepPrefab() = default;
	~SheepPrefab() override = default;

	SheepPrefab(const SheepPrefab& other) = delete;
	SheepPrefab(SheepPrefab&& other) noexcept = delete;
	SheepPrefab& operator=(const SheepPrefab& other) = delete;
	SheepPrefab& operator=(SheepPrefab&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
};