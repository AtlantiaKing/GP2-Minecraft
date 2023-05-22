#include "stdafx.h"
#include "Health.h"

Health::Health(int health) : m_MaxHealth{ health }
{
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
		GetScene()->RemoveChild(GetGameObject(), true);
	}
}
