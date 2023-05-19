#include "stdafx.h"
#include "HealthHUD.h"

#include "Components/Health.h"

HealthHUD::HealthHUD(Health* pHealth)
	: m_pHealth{ pHealth }
{
	pHealth->OnHealthChange.AddListener(this);
}

HealthHUD::~HealthHUD()
{
	if(m_pHealth) m_pHealth->OnHealthChange.RemoveListener(this);
}

void HealthHUD::Notify(const int& health)
{
	const auto& pChildren{ m_pGameObject->GetChildren<GameObject>() };

	for (int i{}; i < pChildren.size(); ++i)
	{
		std::wstringstream spritePath{};
		spritePath << L"Textures/Health/Heart" << (i < health ? L"Full" : L"Empty") << (i % 2) << L".png";

		pChildren[i]->GetComponent<SpriteComponent>()->SetTexture(spritePath.str());
	}
}

void HealthHUD::OnSubjectDestroy()
{
	m_pHealth = nullptr;
}
