#pragma once

#include "Observer/Observer.h"

class LivingEntity : public BaseComponent, Observer<int>
{
public:
	LivingEntity(const XMFLOAT3& hitboxDimensions);
	virtual ~LivingEntity() = default;

	LivingEntity(const LivingEntity& other) = delete;
	LivingEntity(LivingEntity&& other) noexcept = delete;
	LivingEntity& operator=(const LivingEntity& other) = delete;
	LivingEntity& operator=(LivingEntity&& other) noexcept = delete;

	virtual void OnHit(int health) = 0;
	virtual void Notify(const int& health) override;

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext& sceneContext) override;

	virtual void UpdateState() = 0;
	virtual void UpdateMovement(float elapsedSec) = 0;
	virtual void InitMaterials() = 0;

	virtual void EntityUpdate(const SceneContext&) {};

	void SetHitMaterial(BaseMaterial* pHitMaterial);

	unsigned int m_State{ 0 };
	bool m_IsAttacked{};

	float m_RunSpeed{ 2.0f };
	float m_AttackedSpeedBoost{ 1.0f };

	XMFLOAT3 m_HitboxHalfDimensions{};

	ModelAnimator* m_pAnimator{};
	RigidBodyComponent* m_pRb{};

	inline float GetRayDistance() const { return m_RayTestDistance; }
	inline float GetJumpForce() const { return m_JumpForce; }
private:
	void UpdateRotation();
	void RootUpdateState();

	void SetNewRotationTimer();
	void SetNewStateTimer();

	void RootInitMaterials();

	void Rotate(float elapsedSec);

	float m_StateTime{};
	float m_RotationTime{};
	float m_AttackTime{};

	float m_TimeUntilRest{ 5.0f };
	float m_TimeUntilDefaultColor{ 0.5f };

	float m_MinTimeBetweenStates{ 1.0f};
	float m_MaxTimeBetweenStates{ 7.0f };
	float m_TimeUntilStateChange{};

	float m_MinTimeBetweenRotation{ 1.0f };
	float m_MaxTimeBetweenRotation{ 15.0f };
	float m_TimeUntilRotation{};
	float m_GoToRotation{};
	float m_CurRotation{};

	const float m_RayTestDistance{ 0.1f };
	const float m_JumpForce{ 2.0f };

	BaseMaterial* m_pDefaultMaterial{};
	BaseMaterial* m_pHitMaterial{};
};

