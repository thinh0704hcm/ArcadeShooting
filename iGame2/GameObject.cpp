#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"

/*
	Initialize game object 
*/
CGameObject::CGameObject(float x, float y, LPTEXTURE uptex, LPTEXTURE downtex, LPTEXTURE lefttex, LPTEXTURE righttex)
{
	this->x = x;
	this->y = y;
	this->upTexture = uptex;
    this->upTexture = downtex;
    this->upTexture = lefttex;
    this->upTexture = righttex;
}

void CGameObject::Render(LPTEXTURE texture)
{
	CGame::GetInstance()->Draw(x, y, texture);
}

CGameObject::~CGameObject()
{
	if (upTexture != NULL) delete upTexture;
    if (downTexture != NULL) delete downTexture;
    if (leftTexture != NULL) delete leftTexture;
    if (rightTexture != NULL) delete rightTexture;
}

#define Tank_VX 0.1f
#define Tank_WIDTH 14
#define Enemy_WIDTH 40

void CTank::Update(DWORD dt)
{
    if (isDestroyed()) return;
    static bool spacePressed = false;
    static DWORD lastFireTime = 0;
    DWORD currentTime = (DWORD)GetTickCount64();

    // Process WASD input for movement
    if (GetAsyncKeyState(0x57)) y -= vy * dt; // W key
    if (GetAsyncKeyState(0x53)) y += vy * dt; // S key
    if (GetAsyncKeyState(0x41)) x -= vx * dt; // A key
    if (GetAsyncKeyState(0x44)) x += vx * dt; // D key

    // Process space key for bullet spawn
    if (GetAsyncKeyState(VK_SPACE))
    {
        if (!spacePressed || (currentTime - lastFireTime > 200)) // Fire every 200ms
        {
            SpawnBullet();
            spacePressed = true;
            lastFireTime = currentTime;
        }
    }
    else
    {
        spacePressed = false;
    }

    // Ensure the Tank stays within the screen bounds
    int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
    int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();

    if (x < 0) x = 0;
    if (x > BackBufferWidth - Tank_WIDTH) x = (float)(BackBufferWidth - Tank_WIDTH);
    if (y < 0) y = 0;
    if (y > BackBufferHeight - Tank_WIDTH) y = (float)(BackBufferHeight - Tank_WIDTH);
}

void CEnemy::Update(DWORD dt)  
{  
   if (isDestroyed()) return;  

   DWORD currentTime = (DWORD)GetTickCount64();  // Cast to DWORD to avoid C4244 warning

   // Move Enemy horizontally  
   x += vx * dt;  

   // Fire bullet at intervals  
   if (currentTime - lastFireTime > 2000) // Fire every 2 seconds  
   {  
       SpawnBullet();  
       lastFireTime = currentTime;  
   }  

   // Ensure the Enemy stays within the screen bounds and bounce back  
   int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();  

   if (x < 0)  
   {  
       x = 0;  
       vx = -vx;  
   }  
   if (x > BackBufferWidth - Enemy_WIDTH)  
   {  
       x = (float)(BackBufferWidth - Enemy_WIDTH);  
       vx = -vx;  
   }  
}

void CTank::SpawnBullet()  
{  
   float bulletSpeed = 0.5f;  
   float bulletVx = 0, bulletVy = 0;  
   if (vx != 0)  
   {  
       bulletVx = vx / abs(vx) * bulletSpeed;  
   }  
   else if (vy != 0)  
   {  
       bulletVy = vy / abs(vy) * bulletSpeed;  
   }  
   CBullet* bullet = new CBullet(x, y, bulletVx, bulletVy, bulletUpTexture, bulletDownTexture, bulletLeftTexture, bulletRightTexture);  
   CGame::GetInstance()->AddBullet(bullet,0);  
}

void CEnemy::SpawnBullet()
{
    float bulletSpeed = 0.5f;
    float bulletVx = 0, bulletVy = 0;
    if (vx != 0)
    {
        bulletVx = vx / abs(vx) * bulletSpeed;
    }
    else if (vy != 0)
    {
        bulletVy = vy / abs(vy) * bulletSpeed;
    }
    CBullet* bullet = new CBullet(x, y, bulletVx, bulletVy, bulletUpTexture, bulletDownTexture, bulletLeftTexture, bulletRightTexture);
    CGame::GetInstance()->AddBullet(bullet, CGame::GetInstance()->GetEnemyIndex(this));
}

void CBullet::Update(DWORD dt)
{
	if (isDestroyed) return;
	y += vy * dt;
    if (y < 0 || y > CGame::GetInstance()->GetBackBufferHeight())
    {
        isDestroyed = true;
        return;
    }
    CTank* check = CGame::GetInstance()->GetTank();

    if (check != NULL && !check->isDestroyed() && check->CollisionCheck(x, y))
    {
        isDestroyed = true;
        return;
        //Stop game

    }
	for (int i = 0; i < 8; i++)
	{
        CEnemy* check = CGame::GetInstance()->GetEnemy(i);
		if (check != NULL && !check->isDestroyed() && check->CollisionCheck(x, y))
		{
			isDestroyed = true;
            return;
		}
	}

}

void CTank::Render()
{
	if (isDestroyed()) return;
    if (vx == 0)
		if (vy > 0)
			CGameObject::Render(downTexture);
		else
			CGameObject::Render(upTexture);
	else if (vx < 0)
		CGameObject::Render(leftTexture);
	else
		CGameObject::Render(rightTexture);
}

void CBullet::Render()
{
    if (isDestroyed) return;
    if (vx == 0)
        if (vy > 0)
            CGameObject::Render(downTexture);
        else
            CGameObject::Render(upTexture);
    else if (vx < 0)
        CGameObject::Render(leftTexture);
    else
        CGameObject::Render(rightTexture);
}
