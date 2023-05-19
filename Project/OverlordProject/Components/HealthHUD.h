#pragma once

#include "Components/BaseComponent.h"
#include "Observer/Observer.h"

class Health;

class HealthHUD final : public BaseComponent, public Observer<int>
{
public:
	HealthHUD(Health* pHealth);
	virtual ~HealthHUD();

	HealthHUD(const HealthHUD& other) = delete;
	HealthHUD(HealthHUD&& other) noexcept = delete;
	HealthHUD& operator=(const HealthHUD& other) = delete;
	HealthHUD& operator=(HealthHUD&& other) noexcept = delete;

	virtual void Notify(const int& health) override;
	virtual void OnSubjectDestroy() override;

protected:
	virtual void Initialize(const SceneContext&) override {};

private:
	Health* m_pHealth{};
	XMFLOAT2 m_StartPos{};
};

