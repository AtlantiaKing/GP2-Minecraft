#pragma once

#include "Components/BaseComponent.h"
#include "Observer/Subject.h"

class Health final : public BaseComponent
{
public:
	Health() = default;
	Health(int health);
	virtual ~Health() = default;

	Health(const Health& other) = delete;
	Health(Health&& other) noexcept = delete;
	Health& operator=(const Health& other) = delete;
	Health& operator=(Health&& other) noexcept = delete;

	Subject<int> OnHealthChange{};
	Subject<GameObject> OnDeath{};

	void Heal(int amount);
	void Damage(int amount);

	int GetMaxHealth() const { return m_MaxHealth; }

protected:
	virtual void Initialize(const SceneContext&) override {};

private:
	const int m_MaxHealth{ 20 };
	int m_Health{ m_MaxHealth };
};

