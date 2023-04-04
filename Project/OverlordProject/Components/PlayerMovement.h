#pragma once
class PlayerMovement final : public BaseComponent
{
public:
	PlayerMovement(RigidBodyComponent* pPlayer);
	virtual ~PlayerMovement() = default;

	PlayerMovement(const PlayerMovement& other) = delete;
	PlayerMovement(PlayerMovement&& other) noexcept = delete;
	PlayerMovement& operator=(const PlayerMovement& other) = delete;
	PlayerMovement& operator=(PlayerMovement&& other) noexcept = delete;
protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext& sceneContext) override;
private:
	void UpdateRotation(const SceneContext& sceneContext) const;
	void UpdateVelocity(const SceneContext& sceneContext) const;

	RigidBodyComponent* m_pPlayer{};

	float m_RotateSpeed{ 0.005f };
	float m_MoveSpeed{ 4.0f };
	float m_JumpForce{ 7.0f };
};

