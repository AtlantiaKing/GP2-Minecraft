#include "stdafx.h"
#include "Achievement.h"

Achievement::Achievement(const std::wstring& pTexture)
	: m_pTexturePath { pTexture }
{
}

void Achievement::SetTexture(const std::wstring& pTexture)
{
	m_pTexturePath = pTexture;
	m_pSprite = AddComponent(new SpriteComponent{ pTexture });
}

void Achievement::Show()
{
	m_IsShowing = true;
}

void Achievement::Initialize(const SceneContext& sceneContext)
{
	m_pSprite = AddComponent(new SpriteComponent{ m_pTexturePath });

	constexpr float referenceScreen{ 1920.0f };
	const float screenScale{ sceneContext.windowWidth / referenceScreen };
	GetTransform()->Scale(screenScale);
}

void Achievement::Update(const SceneContext& sceneContext)
{
	if (m_IsShowing)
	{
		if (m_CurPos > m_GoToPos) m_CurPos -= sceneContext.pGameTime->GetElapsed() * m_MoveSpeed;
		else
		{
			m_CurTime += sceneContext.pGameTime->GetElapsed();
			if (m_CurTime > m_ShowTime)
			{
				m_IsShowing = false;
				m_CurTime = 0.0f;
			}
		}
	}
	else
	{
		if (m_CurPos < m_StartPos) m_CurPos += sceneContext.pGameTime->GetElapsed() * m_MoveSpeed;
	}

	constexpr float referenceScreen{ 1920.0f };
	const float screenScale{ sceneContext.windowWidth / referenceScreen };
	GetTransform()->Translate(m_CurPos * screenScale, m_YPos * screenScale, 0.0f);
}
