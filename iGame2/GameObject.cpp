#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

/*
	Initialize game object 
*/
CGameObject::CGameObject(float x, float y, LPTEXTURE tex)
{
	this->x = x;
	this->y = y;
	this->texture = tex;
}

void CGameObject::Render()
{
	CGame::GetInstance()->Draw(x, y, texture);
}

CGameObject::~CGameObject()
{
	if (texture != NULL) delete texture;
}

#define SHIP_VX 0.1f
#define SHIP_WIDTH 14
#define UFO_WIDTH 20

void CShip::Update(DWORD dt)
{
	if (isDestroyed) return;
    static bool spacePressed = false;

    // Process WASD input for movement
    if (GetAsyncKeyState(0x57)) y -= vy * dt; // W key
    if (GetAsyncKeyState(0x53)) y += vy * dt; // S key
    if (GetAsyncKeyState(0x41)) x -= vx * dt; // A key
    if (GetAsyncKeyState(0x44)) x += vx * dt; // D key

    // Process space key for bullet spawn
    if (GetAsyncKeyState(VK_SPACE))
    {
        if (!spacePressed)
        {
            SpawnBullet();
            spacePressed = true;
        }
    }
    else
    {
        spacePressed = false;
    }

    // Ensure the ship stays within the screen bounds
    int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
    int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();

    if (x < 0) x = 0;
    if (x > BackBufferWidth - SHIP_WIDTH) x = (float)(BackBufferWidth - SHIP_WIDTH);
    if (y < 0) y = 0;
    if (y > BackBufferHeight - SHIP_WIDTH) y = (float)(BackBufferHeight - SHIP_WIDTH);
}

void CUFO::Update(DWORD dt)  
{  
   if (isDestroyed) return;  

   DWORD currentTime = (DWORD)GetTickCount64();  // Cast to DWORD to avoid C4244 warning

   // Move UFO horizontally  
   x += vx * dt;  

   // Fire bullet at intervals  
   if (currentTime - lastFireTime > 1000) // Fire every 2 seconds  
   {  
       SpawnBullet();  
       lastFireTime = currentTime;  
   }  

   // Ensure the UFO stays within the screen bounds and bounce back  
   int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();  

   if (x < 0)  
   {  
       x = 0;  
       vx = -vx;  
   }  
   if (x > BackBufferWidth - UFO_WIDTH)  
   {  
       x = (float)(BackBufferWidth - UFO_WIDTH);  
       vx = -vx;  
   }  
}

void CShip::SpawnBullet()
{
    float bulletSpeed = 0.5f;
    CBullet* bullet = new CBullet(x, y-150, 0, -bulletSpeed, teamBulletTexture);
    CGame::GetInstance()->AddBullet(bullet);
}

void CUFO::SpawnBullet()
{
    float bulletSpeed = 0.3f;
    CBullet* bullet = new CBullet(x, y+150, 0, bulletSpeed, enemyBulletTexture);
    CGame::GetInstance()->AddBullet(bullet);
}

void CBullet::Update(DWORD dt)
{
	if (isDestroyed) return;
	y += vy * dt;
    CShip* check = CGame::GetInstance()->GetShip();
    if (check != NULL && !check->isDestroyed && check->CollisionCheck(x, y))
    {
        isDestroyed = true;
        //Stop game

    }
	for (int i = 0; i < 50; i++)
	{
        CUFO* check = CGame::GetInstance()->GetUFO(i);
		if (check != NULL && !check->isDestroyed && check->CollisionCheck(x, y))
		{
			isDestroyed = true;
		}
	}

}

void CShip::Render()
{
	if (isDestroyed) return;
	CGameObject::Render();
}

void CUFO::Render()
{
    if (isDestroyed) return;
    CGameObject::Render();
}

void CBullet::Render()
{
    if (isDestroyed) return;
    CGameObject::Render();
}
