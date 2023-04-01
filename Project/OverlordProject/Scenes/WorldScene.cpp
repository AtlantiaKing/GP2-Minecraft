#include "stdafx.h"
#include "WorldScene.h"

#include "Components/WorldComponent.h"

void WorldScene::Initialize()
{
	// Create player

	// Create world generator
	GameObject* pWorld{ AddChild(new GameObject{}) };
	pWorld->AddComponent(new WorldComponent{ m_SceneContext });
}

void WorldScene::Update()
{
}

void WorldScene::Draw()
{
}

void WorldScene::OnGUI()
{
}
