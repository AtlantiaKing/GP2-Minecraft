#pragma once
class PostDeath : public PostProcessingMaterial
{
public:
	PostDeath();
	~PostDeath() override = default;
	PostDeath(const PostDeath& other) = delete;
	PostDeath(PostDeath&& other) noexcept = delete;
	PostDeath& operator=(const PostDeath& other) = delete;
	PostDeath& operator=(PostDeath other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};

