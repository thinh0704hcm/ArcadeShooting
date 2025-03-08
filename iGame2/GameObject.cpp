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
    this->downTexture = downtex;
    this->leftTexture = lefttex;
    this->rightTexture = righttex;
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

#define TANK_VELOCITY 0.08f
#define BULLET_OFFSET 15
#define TANK_WIDTH 14
#define ENEMY_WIDTH 14

void CTank::Update(DWORD dt)
{
    if (isDestroyed()) return;
    static bool spacePressed = false;
    static DWORD lastFireTime = 0;
    DWORD currentTime = (DWORD)GetTickCount64();

    if (isDestroyed()) return;

    if (GetAsyncKeyState(0x57)) { // W key
        vy = -TANK_VELOCITY;
        vx = 0;
    }
    else if (GetAsyncKeyState(0x53)) { // S key
        vy = TANK_VELOCITY;
        vx = 0;
    }
    else if (GetAsyncKeyState(0x41)) { // A key
        vx = -TANK_VELOCITY;
        vy = 0;
    }
    else if (GetAsyncKeyState(0x44)) { // D key
        vx = TANK_VELOCITY;
        vy = 0;
    }

    x += vx * dt;
    y += vy * dt;

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
    if (x > BackBufferWidth - TANK_WIDTH) x = (float)(BackBufferWidth - TANK_WIDTH);
    if (y < 0) y = 0;
    if (y > BackBufferHeight - TANK_WIDTH) y = (float)(BackBufferHeight - TANK_WIDTH);
}

void CEnemy::Update(DWORD dt)
{
    if (isDestroyed()) return;

    DWORD currentTime = GetTickCount64();

    // Check if it's time to change direction
    if (currentTime - lastDirectionChange > directionChangeInterval)
    {
        ChangeDirection();
        lastDirectionChange = currentTime;
        directionChangeInterval = 2000 + (rand() % 3000); // Set new random interval
    }

    // Move Enemy
    x += vx * dt;
    y += vy * dt;

    // Handle screen boundaries
    int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
    int BackBufferHeight = CGame::GetInstance()->GetBackBufferHeight();

    // Bounce off screen edges
    if (x < 0)
    {
        x = 0;
        vx = -vx;
    }
    if (x > BackBufferWidth - 40)  // 40 is enemy width
    {
        x = (float)(BackBufferWidth - 40);
        vx = -vx;
    }
    if (y < 0)
    {
        y = 0;
        vy = -vy;
    }
    if (y > BackBufferHeight - 40)  // 40 is enemy height
    {
        y = (float)(BackBufferHeight - 40);
        vy = -vy;
    }

    // Fire bullet at intervals
    if (currentTime - lastFireTime > 2000) // Fire every 2 seconds
    {
        SpawnBullet();
        lastFireTime = currentTime;
    }
}

// Add this new method to implement direction changes
void CEnemy::ChangeDirection()
{
    float speed = TANK_VELOCITY;
    int direction = rand() % 4;  // 4 directions

    switch (direction)
    {
    case 0: // Right
        vx = speed;
        vy = 0;
        break;
    case 1: // Left
        vx = -speed;
        vy = 0;
        break;
    case 2: // Up
        vx = 0;
        vy = -speed;
        break;
    case 3: // Down
        vx = 0;
        vy = speed;
        break;
    }
}

void CTank::SpawnBullet()  
{  
    float bulletSpeed = 0.5f;
    float bulletX = x;
    float bulletY = y;
    float bulletVx = 0, bulletVy = 0;

    // Calculate bullet spawn position based on tank's direction
    if (vx > 0) // Moving right
    {
        bulletX = x + TANK_WIDTH + BULLET_OFFSET;
        bulletY = y + (TANK_WIDTH / 2);
        bulletVx = bulletSpeed;
        bulletVy = 0;
    }
    else if (vx < 0) // Moving left
    {
        bulletX = x - BULLET_OFFSET;
        bulletY = y + (TANK_WIDTH / 2);
        bulletVx = -bulletSpeed;
        bulletVy = 0;
    }
    else if (vy > 0) // Moving down
    {
        bulletX = x + (TANK_WIDTH / 2);
        bulletY = y + TANK_WIDTH + BULLET_OFFSET;
        bulletVx = 0;
        bulletVy = bulletSpeed;
    }
    else // Moving up (or default)
    {
        bulletX = x + (TANK_WIDTH / 2);
        bulletY = y - BULLET_OFFSET;
        bulletVx = 0;
        bulletVy = -bulletSpeed;
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
    CGame::GetInstance()->AddBullet(bullet, CGame::GetInstance()->GetEnemyIndex(this)+1);
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
