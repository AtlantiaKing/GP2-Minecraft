#pragma once

class PostUnderWater : public PostProcessingMaterial
{
public:
	PostUnderWater();
	~PostUnderWater() override = default;
	PostUnderWater(const PostUnderWater& other) = delete;
	PostUnderWater(PostUnderWater&& other) noexcept = delete;
	PostUnderWater& operator=(const PostUnderWater& other) = delete;
	PostUnderWater& operator=(PostUnderWater&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};

