#include "stdafx.h"
#include "Health.h"

void Health::Heal(int amount)
{
	m_Health = (m_Health + amount) % m_MaxHealth;

	OnHealthChange.Notify(m_Health);
}

void Health::Damage(int amount)
{
	m_Health -= amount;
	if (m_Health <= 0) OnDeath.Notify(*GetGameObject());

	OnHealthChange.Notify(m_Health);
}
