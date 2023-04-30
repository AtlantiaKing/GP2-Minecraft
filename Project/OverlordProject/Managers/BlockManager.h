#pragma once
#include <Misc/World/WorldData.h>

class BlockManager final : public Singleton<BlockManager>
{
public:

	BlockManager(const BlockManager& other) = delete;
	BlockManager(BlockManager&& other) noexcept = delete;
	BlockManager& operator=(const BlockManager& other) = delete;
	BlockManager& operator=(BlockManager&& other) noexcept = delete;

	Block* GetBlock(const std::string& identifier) const;
	Block* GetBlock(BlockType type) const;

	const Biome& GetBiome(const std::string& identifier) const;
protected:
	virtual void Initialize() override {};

private:
	friend Singleton<BlockManager>;
	BlockManager();
	~BlockManager();

	std::unordered_map<std::string, Block*> m_pBlocksByIdentifier{};
	std::unordered_map<BlockType, Block*> m_pBlocksByType{};

	std::unordered_map<std::string, Biome> m_BiomesByIdentifier{};
};

