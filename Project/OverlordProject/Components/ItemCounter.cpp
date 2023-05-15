#include "stdafx.h"
#include "ItemCounter.h"

void ItemCounter::SetAmount(int amount)
{
	m_Text = amount > 1 ? std::to_string(amount) : "";
}

void ItemCounter::Initialize(const SceneContext&)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"Fonts/Minecraft_32.fnt");
}

void ItemCounter::Update(const SceneContext&)
{
	float numberSize{};
	for (wchar_t c : StringUtil::utf8_decode(m_Text))
	{
		numberSize += m_pFont->GetMetric(c).width;
	}
	constexpr float pixelSize{ 4.0f };
	constexpr float offset{ -pixelSize / 2.0f };

	const auto& worldPos{ GetTransform()->GetWorldPosition() };
	const XMFLOAT2 pos{ worldPos.x - numberSize + offset, worldPos.y - m_pFont->GetSize()};
	const XMFLOAT2 backgroundPos{ pos.x + pixelSize, pos.y + pixelSize };

	constexpr XMFLOAT4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
	constexpr XMFLOAT4 backgroundColor{ 0.2f, 0.2f, 0.2f, 1.0f };

	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), backgroundPos, backgroundColor);
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), pos, color);
}
