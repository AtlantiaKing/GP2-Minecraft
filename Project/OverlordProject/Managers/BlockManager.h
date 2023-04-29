#pragma once
#include <Misc/World/WorldData.h>

class BlockManager final : public Singleton<BlockManager>
{
public:

	BlockManager(const BlockManager& other) = delete;
	BlockManager(BlockManager&& other) noexcept = delete;
	BlockManager& operator=(const BlockManager& other) = delete;
	BlockManager& operator=(BlockManager&& other) noexcept = delete;

	Block* GetBlock(const std::string& identifier);
protected:
	virtual void Initialize() override {};

private:
	friend Singleton<BlockManager>;
	BlockManager();
	virtual ~BlockManager() = default;

	std::unordered_map<std::string, Block*> m_pBlocks{};
};

