#pragma once
class Achievement final : public GameObject
{
public:
	Achievement(const std::wstring& pTexture);
	~Achievement() override = default;

	Achievement(const Achievement& other) = delete;
	Achievement(Achievement&& other) noexcept = delete;
	Achievement& operator=(const Achievement& other) = delete;
	Achievement& operator=(Achievement&& other) noexcept = delete;

	void SetTexture(const std::wstring& pTexture);
	void Show();

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	const float m_StartPos{ 1920.0f };
	const float m_YPos{ 20.0f };
	const float m_GoToPos{ 1350.0f };
	const float m_MoveSpeed{ 1000.0f };

	const float m_ShowTime{ 2.0f };

	bool m_IsShowing{};
	float m_CurPos{ m_StartPos };
	float m_CurTime{};

	std::wstring m_pTexturePath{};
	SpriteComponent* m_pSprite{};
};

