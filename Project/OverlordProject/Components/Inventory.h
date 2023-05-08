#pragma once

#include "Components/BaseComponent.h"

#include "Misc/World/WorldData.h"

struct Block;

class Inventory final : public BaseComponent
{
public:
	Inventory() = default;
	virtual ~Inventory() = default;

	Inventory(const Inventory& other) = delete;
	Inventory(Inventory&& other) noexcept = delete;
	Inventory& operator=(const Inventory& other) = delete;
	Inventory& operator=(Inventory&& other) noexcept = delete;

	virtual void Initialize(const SceneContext&) {}

	void Add(BlockType pBlock);
private:
	std::unordered_map<BlockType, int> m_Hotbar{};
};

