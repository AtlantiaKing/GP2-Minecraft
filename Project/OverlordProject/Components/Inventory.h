#pragma once

#include "Components/BaseComponent.h"

#include "Misc/World/WorldData.h"
#include "Observer/Subject.h"

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

	void Add(BlockType block);
	void Remove(BlockType block);
	bool HasOfType(BlockType block) const;
	bool HasEquipedItem() const;
	BlockType GetEquipedItem() const;
	int GetMaxItems() const;
	int GetSlot() const;
	Subject<std::unordered_map<BlockType, int>>& OnInventoryChange() { return m_OnInventoryChange; }

protected:
	virtual void Initialize(const SceneContext&) {};
	virtual void Update(const SceneContext& sceneContext);

private:
	std::unordered_map<BlockType, int> m_Hotbar{};
	Subject<std::unordered_map<BlockType, int>> m_OnInventoryChange{};
	int m_CurSlot{};
	const int m_MaxItems{ 10 };
};

