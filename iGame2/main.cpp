/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102

	SAMPLE 01 - SKELETON CODE

	This sample illustrates how to:

	1/ Re-organize introductory code to an initial skeleton for better scalability
	2/ CGame is a singleton, playing a role of an "engine".
	3/ CGameObject is an abstract class for all game objects
	4/ CTexture is a wrapper class for ID3D10TEXTURE

	NOTE: to create transparent background, download GIMP, then use Color to Alpha feature
================================================================ */

#include <windows.h>
#include <d3d10.h>
#include <d3dx10.h> // Commented out because d3dx10.h is deprecated and not available in newer SDKs
#include <vector>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

#define WINDOW_CLASS_NAME L"Game Window"
#define MAIN_WINDOW_TITLE L"01 - Skeleton"
#define WINDOW_ICON_PATH L"brick.ico"

#define TEXTURE_PATH_TANK_UP L"yellow_tank_up.png"
#define TEXTURE_PATH_TANK_DOWN L"yellow_tank_down.png"
#define TEXTURE_PATH_TANK_LEFT L"yellow_tank_left.png"
#define TEXTURE_PATH_TANK_RIGHT L"yellow_tank_right.png"

#define TEXTURE_PATH_RED_UP L"red_tank_up.png"
#define TEXTURE_PATH_RED_DOWN L"red_tank_down.png"
#define TEXTURE_PATH_RED_LEFT L"red_tank_left.png"
#define TEXTURE_PATH_RED_RIGHT L"red_tank_right.png"

#define TEXTURE_PATH_GREEN_UP L"green_tank_up.png"
#define TEXTURE_PATH_GREEN_DOWN L"green_tank_down.png"
#define TEXTURE_PATH_GREEN_LEFT L"green_tank_left.png"
#define TEXTURE_PATH_GREEN_RIGHT L"green_tank_right.png"

#define TEXTURE_PATH_WHITE_UP L"white_tank_up.png"
#define TEXTURE_PATH_WHITE_DOWN L"white_tank_down.png"
#define TEXTURE_PATH_WHITE_LEFT L"white_tank_left.png"
#define TEXTURE_PATH_WHITE_RIGHT L"white_tank_right.png"

#define TEXTURE_PATH_BULLET_UP L"bullet_up.png"
#define TEXTURE_PATH_BULLET_DOWN L"bullet_down.png"
#define TEXTURE_PATH_BULLET_LEFT L"bullet_left.png"
#define TEXTURE_PATH_BULLET_RIGHT L"bullet_right.png"


#define BACKGROUND_COLOR D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 320

using namespace std;

#define Tank_START_X 300.0f
#define Tank_START_Y 300.0f
#define	TANK_VELOCITY 0.1f

LPTEXTURE texTankUp = NULL;
LPTEXTURE texTankDown = NULL;
LPTEXTURE texTankLeft = NULL;
LPTEXTURE texTankRight = NULL;

LPTEXTURE texRedUp = NULL;
LPTEXTURE texRedDown = NULL;
LPTEXTURE texRedLeft = NULL;
LPTEXTURE texRedRight = NULL;

LPTEXTURE texGreenUp = NULL;
LPTEXTURE texGreenDown = NULL;
LPTEXTURE texGreenLeft = NULL;
LPTEXTURE texGreenRight = NULL;

LPTEXTURE texWhiteUp = NULL;
LPTEXTURE texWhiteDown = NULL;
LPTEXTURE texWhiteLeft = NULL;
LPTEXTURE texWhiteRight = NULL;

LPTEXTURE texBulletUp = NULL;
LPTEXTURE texBulletDown = NULL;
LPTEXTURE texBulletLeft = NULL;
LPTEXTURE texBulletRight = NULL;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void LoadResources()
{
	CGame* game = CGame::GetInstance();
	texTankUp = game->LoadTexture(TEXTURE_PATH_TANK_UP);
	texTankDown = game->LoadTexture(TEXTURE_PATH_TANK_DOWN);
	texTankLeft = game->LoadTexture(TEXTURE_PATH_TANK_LEFT);
	texTankRight = game->LoadTexture(TEXTURE_PATH_TANK_RIGHT);

	texRedUp = game->LoadTexture(TEXTURE_PATH_RED_UP);
	texRedDown = game->LoadTexture(TEXTURE_PATH_RED_DOWN);
	texRedLeft = game->LoadTexture(TEXTURE_PATH_RED_LEFT);
	texRedRight = game->LoadTexture(TEXTURE_PATH_RED_RIGHT);

	texGreenUp = game->LoadTexture(TEXTURE_PATH_GREEN_UP);
	texGreenDown = game->LoadTexture(TEXTURE_PATH_GREEN_DOWN);
	texGreenLeft = game->LoadTexture(TEXTURE_PATH_GREEN_LEFT);
	texGreenRight = game->LoadTexture(TEXTURE_PATH_GREEN_RIGHT);

	texWhiteUp = game->LoadTexture(TEXTURE_PATH_WHITE_UP);
	texWhiteDown = game->LoadTexture(TEXTURE_PATH_WHITE_DOWN);
	texWhiteLeft = game->LoadTexture(TEXTURE_PATH_WHITE_LEFT);
	texWhiteRight = game->LoadTexture(TEXTURE_PATH_WHITE_RIGHT);

	texBulletUp = game->LoadTexture(TEXTURE_PATH_BULLET_UP);
	texBulletDown = game->LoadTexture(TEXTURE_PATH_BULLET_DOWN);
	texBulletLeft = game->LoadTexture(TEXTURE_PATH_BULLET_LEFT);
	texBulletRight = game->LoadTexture(TEXTURE_PATH_BULLET_RIGHT);

	CGame::GetInstance()->AddTank(new CTank(Tank_START_X, Tank_START_Y, 0, 0,
		texTankUp, texTankDown, texTankLeft, texTankRight,
		texBulletUp, texBulletDown, texBulletLeft, texBulletRight));

	srand(time(NULL));

	for (int i = 0; i < 8; i++)
	{
		float margin = 1.0f; // Define a margin to avoid spawning at the bounds
		float randomX = margin + static_cast<float>(rand() % static_cast<int>(SCREEN_WIDTH - 2 * margin));
		float randomY = static_cast<float>(i * 90);
		float randomVX, randomVY;
		int direction = rand() % 4;

		switch (direction)
		{
		case 0: // Right
			randomVX = TANK_VELOCITY;
			randomVY = 0.0f;
			break;
		case 1: // Left
			randomVX = -TANK_VELOCITY;
			randomVY = 0.0f;
			break;
		case 2: // Up
			randomVX = 0.0f;
			randomVY = -TANK_VELOCITY;  // For screen coordinates, negative Y is upward.
			break;
		case 3: // Down
			randomVX = 0.0f;
			randomVY = TANK_VELOCITY;
			break;
		default:
			// Fallback (should never occur)
			randomVX = 0.0f;
			randomVY = 0.0f;
			break;
		}
		CGame::GetInstance()->AddEnemy(new CEnemy(randomX, randomY, randomVX, randomVY, rand() % 3 + 1,
			texRedUp, texRedDown, texRedLeft, texRedRight,
			texGreenUp, texGreenDown, texGreenLeft, texGreenRight,
			texWhiteUp, texWhiteDown, texWhiteLeft, texWhiteRight,
			texBulletUp, texBulletDown, texBulletLeft, texBulletRight));
	}
}

void Update(DWORD dt)
{
	CGame* game = CGame::GetInstance();
	game->Update(dt);

	//Showing health in titlebar
	DebugOutTitle(L"Health : %i", game->GetTank()->GetHealth());
	DebugOut(L"Tank X : %i, Y : %i\n", game->GetTank()->GetX(), game->GetTank()->GetY());
	for (int i = 0; i < 7; i++)
	{
		CEnemy* Enemy = game->GetEnemy(i);
		if (Enemy != nullptr) DebugOut(L"Enemy %i X : %i, Y : %i\n", i, Enemy->GetX(), Enemy->GetY());
	}

}

void Render()
{
	CGame* g = CGame::GetInstance();

	ID3D10Device* pD3DDevice = g->GetDirect3DDevice();
	IDXGISwapChain* pSwapChain = g->GetSwapChain();
	ID3D10RenderTargetView* pRenderTargetView = g->GetRenderTargetView();
	ID3DX10Sprite* spriteHandler = g->GetSpriteHandler();

	if (pD3DDevice != NULL)
	{
		pD3DDevice->ClearRenderTargetView(pRenderTargetView, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DX10_SPRITE_SORT_TEXTURE);

		FLOAT NewBlendFactor[4] = { 0,0,0,0 };
		pD3DDevice->OMSetBlendState(g->GetAlphaBlending(), NewBlendFactor, 0xffffffff);

		g->GetTank()->Render();

		for (int i = 0; i < 7; i++)
		{
			CEnemy* Enemy = g->GetEnemy(i);
			if (Enemy != nullptr) Enemy->Render();
		}

		for (int i = 0; i < 8; i++)
		{
			CBullet* bullet = g->GetBullet(i);
			if (bullet != nullptr) bullet->Render();
		}

		spriteHandler->End();
		pSwapChain->Present(0, 0);
	}
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = (HICON)LoadImage(hInstance, WINDOW_ICON_PATH, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		DWORD ErrCode = GetLastError();
		DebugOut(L"[ERROR] CreateWindow failed! ErrCode: %d\nAt: %s %d \n", ErrCode, _W(__FILE__), __LINE__);
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	SetDebugWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	ULONGLONG frameStart = GetTickCount64();
	ULONGLONG tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		ULONGLONG now = GetTickCount64();
		ULONGLONG dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update((DWORD)dt);
			Render();

		}
		else
			Sleep((DWORD)(tickPerFrame - dt));
	}
	return 1;
}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame* game = CGame::GetInstance();
	game->Init(hWnd);

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	LoadResources();

	Run();

	return 0;
}