#pragma once
class PostDark : public PostProcessingMaterial
{
public:
	PostDark();
	~PostDark() override = default;
	PostDark(const PostDark& other) = delete;
	PostDark(PostDark&& other) noexcept = delete;
	PostDark& operator=(const PostDark& other) = delete;
	PostDark& operator=(PostDark&& other) noexcept = delete;

protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};

