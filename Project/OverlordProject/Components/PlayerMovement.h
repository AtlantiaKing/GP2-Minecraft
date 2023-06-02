#pragma once
class PlayerMovement final : public BaseComponent
{
public:
	PlayerMovement() = default;
	virtual ~PlayerMovement() = default;

	PlayerMovement(const PlayerMovement& other) = delete;
	PlayerMovement(PlayerMovement&& other) noexcept = delete;
	PlayerMovement& operator=(const PlayerMovement& other) = delete;
	PlayerMovement& operator=(PlayerMovement&& other) noexcept = delete;

	void SetUnderWater(bool isUnderWater);
	bool IsSwimming() const { return m_IsUnderWater; }

	void AddVelocity(float x, float y, float z);

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Update(const SceneContext& sceneContext) override;

private:
	void UpdateRotation(const SceneContext& sceneContext) const;
	void UpdateVelocity(const SceneContext& sceneContext);

	ControllerComponent* m_pController{};

	XMFLOAT3 m_OneFrameVelocity{};

	bool m_Spawned{};

	float m_SprintFOV{ 95.0f };
	float m_FOV{ 80.0f };

	float m_RotateSpeed{ 0.005f };

	float m_MoveSpeed{ 4.317f };
	float m_SprintSpeed{ 5.612f };
	float m_SwimSpeed{ 3.0f };

	float m_JumpForce{ 7.0f };
	float m_SwimForce{ 4.0f };

	float m_MinUnderWaterVelocity{ -1.0f };
	float m_MaxUnderWaterVelocity{ 2.0f };
	float m_UnderWaterGravity{ -4.0f };

	bool m_IsGrounded{};
	bool m_IsUnderWater{};

	XMFLOAT3 m_Velocity{};
};

