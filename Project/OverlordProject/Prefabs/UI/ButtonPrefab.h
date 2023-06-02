#pragma once
#include "Observer/Subject.h"

class ButtonPrefab final : public GameObject
{
public:
	ButtonPrefab(const std::wstring& texturePath);
	~ButtonPrefab() override = default;

	ButtonPrefab(const ButtonPrefab& other) = delete;
	ButtonPrefab(ButtonPrefab&& other) noexcept = delete;
	ButtonPrefab& operator=(const ButtonPrefab& other) = delete;
	ButtonPrefab& operator=(ButtonPrefab&& other) noexcept = delete;

	Subject<const ButtonPrefab&> OnClick{};

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	std::wstring m_TexturePath{};
	XMFLOAT2 m_TextureSize{};
};

