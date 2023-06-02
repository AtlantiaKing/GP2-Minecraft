#include "stdafx.h"
#include "MainGame.h"

// Scenes
#include "Scenes/WorldScene.h"
#include "Scenes/MainMenuScene.h"

// Managers
#include "Managers/BlockManager.h"

MainGame::~MainGame()
{
	BlockManager::Destroy();
}

//Game is preparing
void MainGame::OnGamePreparing(GameContext& gameContext)
{
	//Here you can change some game settings before engine initialize
	gameContext.windowWidth = 1280;
	gameContext.windowHeight = 720;

	gameContext.windowTitle = L"GP2 - Exam Project (2023) | (2DAE15) De Keukelaere Sander";

	m_pGameContext = &gameContext;

	BlockManager::Create(gameContext);
}

void MainGame::Initialize()
{
	// Nice looking seeds:
	//  3 beach line with entrance to a valley
	//  8 spawn on land (best seed to play on)
	//  6 good looking mountain but needs render distance bigger then 15
	//srand(static_cast<unsigned int>(time(nullptr)));
	srand(8);

	SceneManager::Get()->AddGameScene(new MainMenuScene());
	SceneManager::Get()->AddGameScene(new WorldScene());
}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

		if (wParam == VK_F11)
		{
			m_Fullscreen = !m_Fullscreen;
			if (m_Fullscreen)
			{
				// Remove title bar and borders
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);

				// Set window position and size to cover entire screen
				SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
			}
			else
			{
				// Restore title bar and borders
				SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

				// Calculate dimensions of window based on specified width and height
				RECT r = { 0, 0, LONG(m_pGameContext->windowWidth), LONG(m_pGameContext->windowHeight) };
				AdjustWindowRect(&r, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX, false);
				const auto winWidth = r.right - r.left;
				const auto winHeight = r.bottom - r.top;

				// Calculate position of window to center on screen
				const int x = (GetSystemMetrics(SM_CXSCREEN) - winWidth) / 2;
				const int y = (GetSystemMetrics(SM_CYSCREEN) - winHeight) / 2;

				// Set window position and size
				SetWindowPos(hWnd, HWND_TOP, x, y, winWidth, winHeight, SWP_SHOWWINDOW);
			}
		}
	}
	

	return -1;
}
