#include "stdafx.h"
#include "ButtonPrefab.h"

ButtonPrefab::ButtonPrefab(const std::wstring& texturePath)
	: m_TexturePath{ texturePath }
{
}

void ButtonPrefab::Initialize(const SceneContext&)
{
	m_TextureSize = AddComponent(new SpriteComponent{ m_TexturePath, {0.5f, 0.5f} })->GetSize();
}

void ButtonPrefab::Update(const SceneContext& sceneContext)
{
	constexpr int clickButton{ 1 }; // Left mouse button
	if (sceneContext.pInput->IsMouseButton(InputState::pressed, clickButton))
	{
		const auto& scale{ GetTransform()->GetWorldScale() };
		const XMFLOAT2 buttonHalfSize
		{
			m_TextureSize.x * scale.x / 2.0f,
			m_TextureSize.y * scale.y / 2.0f
		};

		POINT mousePos{ sceneContext.pInput->GetMousePosition() };
		ScreenToClient(GetForegroundWindow(), &mousePos);

		const auto& buttonCenter{ GetTransform()->GetWorldPosition() };
		
		if (mousePos.x < buttonCenter.x - buttonHalfSize.x || mousePos.x > buttonCenter.x + buttonHalfSize.x) return;
		if (mousePos.y < buttonCenter.y - buttonHalfSize.y || mousePos.y > buttonCenter.y + buttonHalfSize.y) return;

		OnClick.Notify(*this);
	}
}
