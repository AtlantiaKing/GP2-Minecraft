#pragma once

#include "Components/BaseComponent.h"
#include "Observer/Observer.h"
#include <Misc/World/WorldData.h>

class Inventory;

class ToolbarHUD final : public BaseComponent, public Observer<std::unordered_map<BlockType, int>>
{
public:
	ToolbarHUD(Inventory* pInventory);
	virtual ~ToolbarHUD();

	ToolbarHUD(const ToolbarHUD& other) = delete;
	ToolbarHUD(ToolbarHUD&& other) noexcept = delete;
	ToolbarHUD& operator=(const ToolbarHUD& other) = delete;
	ToolbarHUD& operator=(ToolbarHUD&& other) noexcept = delete;

	virtual void Initialize(const SceneContext&) override {};
	virtual void Notify(const std::unordered_map<BlockType, int>& inventory) override;
	virtual void OnSubjectDestroy() override;
private:
	Inventory* m_pInventory;
};

