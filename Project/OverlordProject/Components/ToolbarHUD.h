#pragma once

#include "Components/BaseComponent.h"
#include "Observer/Observer.h"
#include <Misc/World/WorldData.h>

class Inventory;

class ToolbarHUD final : public BaseComponent, public Observer<std::unordered_map<BlockType, int>>
{
public:
	ToolbarHUD(Inventory* pInventory, const XMFLOAT2& screenSize);
	virtual ~ToolbarHUD();

	ToolbarHUD(const ToolbarHUD& other) = delete;
	ToolbarHUD(ToolbarHUD&& other) noexcept = delete;
	ToolbarHUD& operator=(const ToolbarHUD& other) = delete;
	ToolbarHUD& operator=(ToolbarHUD&& other) noexcept = delete;

	virtual void Notify(const std::unordered_map<BlockType, int>& inventory) override;
	virtual void OnSubjectDestroy() override;

	void SetSelection(GameObject* pSelection);
	
protected:
	virtual void Initialize(const SceneContext&) override {};

private:
	Inventory* m_pInventory{};
	GameObject* m_pSelection{};
	XMFLOAT2 m_ScreenSize{};
};

