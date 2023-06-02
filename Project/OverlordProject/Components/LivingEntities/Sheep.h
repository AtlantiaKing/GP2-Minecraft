#pragma once

#include "LivingEntity.h"

class Sheep : public LivingEntity
{
public:
	Sheep(const XMFLOAT3& hitboxDimensions);
	virtual ~Sheep() = default;

	Sheep(const Sheep& other) = delete;
	Sheep(Sheep&& other) noexcept = delete;
	Sheep& operator=(const Sheep& other) = delete;
	Sheep& operator=(Sheep&& other) noexcept = delete;

	virtual void OnHit(int health) override;

protected:
	virtual void InitMaterials() override;
	virtual void UpdateState() override;
	virtual void UpdateMovement(float elapsedSec);

private:
	enum class SheepState
	{
		Idle,
		Walking
	};
};

