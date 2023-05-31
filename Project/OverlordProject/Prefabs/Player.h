#pragma once

class WorldComponent;
class BlockInteractionComponent;
class BlockBreakParticle;
class PlayerMovement;

class Player final : public GameObject
{
public:
	Player(WorldComponent* pWorld, GameObject* pSelection, BlockBreakParticle* pBlockBreakParticle);
	~Player() override = default;

	Player(const Player& other) = delete;
	Player(Player&& other) noexcept = delete;
	Player& operator=(const Player& other) = delete;
	Player& operator=(Player&& other) noexcept = delete;

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext& sceneContext) override;

private:
	BlockInteractionComponent* m_pInteraction{};
	WorldComponent* m_pWorld{};
	GameObject* m_pSelection{};
	BlockBreakParticle* m_pBlockBreakParticle{};
	ModelAnimator* m_pArmAnimation{};
	PlayerMovement* m_pMovement{};
};
