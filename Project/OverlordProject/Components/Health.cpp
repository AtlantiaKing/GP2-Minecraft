#include "stdafx.h"
#include "Health.h"

Health::Health(int health) : m_MaxHealth{ health }
{
}

void Health::SetDestroyOnDeath(bool destroyOnDeath)
{
	m_DestroyOnDeath = destroyOnDeath;
}

void Health::Heal(int amount)
{
	m_Health = (m_Health + amount) % m_MaxHealth;

	OnHealthChange.Notify(m_Health);
}

void Health::Damage(int amount)
{
	m_Health -= amount;

	OnHealthChange.Notify(m_Health);

	if (m_Health <= 0)
	{
		OnDeath.Notify(*GetGameObject());
		if(m_DestroyOnDeath) GetScene()->RemoveChild(GetGameObject(), true);
	}
}
