#include "stdafx.h"
#include "MainGame.h"

//Game is preparing
void MainGame::OnGamePreparing(GameContext& gameContext)
{
	//Here you can change some game settings before engine initialize
	//gameContext.windowWidth=... (default is 1280)
	//gameContext.windowHeight=... (default is 720)

	gameContext.windowTitle = L"GP2 - Exam Project (2023) | (2DAE15) De Keukelaere Sander";
}

void MainGame::Initialize()
{
	//SceneManager::Get()->AddGameScene(new WorldScene());
}

LRESULT MainGame::WindowProcedureHook(HWND /*hWnd*/, UINT message, WPARAM wParam, LPARAM lParam)
{

	if(message == WM_KEYUP)
	{
		if ((lParam & 0x80000000) != 0x80000000)
			return -1;

		//[F1] Toggle Scene Info Overlay
		if(wParam == VK_F1)
		{
			const auto pScene = SceneManager::Get()->GetActiveScene();
			pScene->GetSceneSettings().Toggle_ShowInfoOverlay();
		}

		//[F2] Toggle Debug Renderer (Global)
		if (wParam == VK_F2)
		{
			DebugRenderer::ToggleDebugRenderer();
			return 0;

		}

		//[F3] Previous Scene
		if (wParam == VK_F3)
		{
			SceneManager::Get()->PreviousScene();
			return 0;

		}

		//[F4] Next Scene
		if (wParam == VK_F4)
		{
			SceneManager::Get()->NextScene();
			return 0;
		}

		//[F5] If PhysX Framestepping is enables > Next Frame	
		if (wParam == VK_F6)
		{
			const auto pScene = SceneManager::Get()->GetActiveScene();
			pScene->GetPhysxProxy()->NextPhysXFrame();
		}
	}
	

	return -1;
}
