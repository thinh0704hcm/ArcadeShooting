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
		orientation = 0;
	}
	else if (GetAsyncKeyState(0x53)) { // S key
		vy = TANK_VELOCITY;
		vx = 0;
		orientation = 1;
	}
	else if (GetAsyncKeyState(0x41)) { // A key
		vx = -TANK_VELOCITY;
		vy = 0;
		orientation = 2;
	}
	else if (GetAsyncKeyState(0x44)) { // D key
		vx = TANK_VELOCITY;
		vy = 0;
		orientation = 3;
	}
	else
	{
		vx = 0;
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
		orientation = 3; // Change orientation to right
	}
	if (x > BackBufferWidth - 7)  // 7 is enemy width
	{
		x = (float)(BackBufferWidth - 7);
		vx = -vx;
		orientation = 2; // Change orientation to left
	}
	if (y < 0)
	{
		y = 0;
		vy = -vy;
		orientation = 1; // Change orientation to down
	}
	if (y > BackBufferHeight - 7)  // 7 is enemy height
	{
		y = (float)(BackBufferHeight - 7);
		vy = -vy;
		orientation = 0; // Change orientation to up
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
	orientation = rand() % 4;  // 4 directions

	switch (orientation)
	{
	case 0: // Up
		vx = 0.0f;
		vy = -TANK_VELOCITY;  // For screen coordinates, negative Y is upward.
		break;
	case 1: // Down
		vx = 0.0f;
		vy = TANK_VELOCITY;
		break;
	case 2: // Left
		vx = -TANK_VELOCITY;
		vy = 0.0f;
		break;
	case 3: // Right
		vx = TANK_VELOCITY;
		vy = 0.0f;
		break;
	default:
		// Fallback (should never occur)
		vx = 0.0f;
		vy = 0.0f;
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
	switch (orientation) // Moving right
	{
	case 3:
		bulletX = x + TANK_WIDTH + BULLET_OFFSET;
		bulletY = y + (TANK_WIDTH / 2);
		bulletVx = bulletSpeed;
		bulletVy = 0;
		break;
	case 2: // Moving left
		bulletX = x - BULLET_OFFSET;
		bulletY = y + (TANK_WIDTH / 2);
		bulletVx = -bulletSpeed;
		bulletVy = 0;
		break;
	case 1:

		bulletX = x + (TANK_WIDTH / 2);
		bulletY = y + TANK_WIDTH + BULLET_OFFSET;
		bulletVx = 0;
		bulletVy = bulletSpeed;
		break;
	default:
		bulletX = x + (TANK_WIDTH / 2);
		bulletY = y - BULLET_OFFSET;
		bulletVx = 0;
		bulletVy = -bulletSpeed;

	}
	CBullet* bullet = new CBullet(x, y, bulletVx, bulletVy, orientation, bulletUpTexture, bulletDownTexture, bulletLeftTexture, bulletRightTexture);
	CGame::GetInstance()->AddBullet(bullet, 0);
}

void CEnemy::SpawnBullet()
{
	float bulletSpeed = 0.5f;
	float bulletX = x;
	float bulletY = y;
	float bulletVx = 0, bulletVy = 0;

	// Calculate bullet spawn position based on tank's direction
	switch (orientation) // Moving right
	{
	case 3:
		bulletX = x + TANK_WIDTH + BULLET_OFFSET;
		bulletY = y + (TANK_WIDTH / 2);
		bulletVx = bulletSpeed;
		bulletVy = 0;
		break;
	case 2: // Moving left
		bulletX = x - BULLET_OFFSET;
		bulletY = y + (TANK_WIDTH / 2);
		bulletVx = -bulletSpeed;
		bulletVy = 0;
		break;
	case 1:

		bulletX = x + (TANK_WIDTH / 2);
		bulletY = y + TANK_WIDTH + BULLET_OFFSET;
		bulletVx = 0;
		bulletVy = bulletSpeed;
		break;
	default:
		bulletX = x + (TANK_WIDTH / 2);
		bulletY = y - BULLET_OFFSET;
		bulletVx = 0;
		bulletVy = -bulletSpeed;

	}
	CBullet* bullet = new CBullet(bulletX, bulletY, bulletVx, bulletVy, orientation, bulletUpTexture, bulletDownTexture, bulletLeftTexture, bulletRightTexture);
	CGame::GetInstance()->AddBullet(bullet, CGame::GetInstance()->GetEnemyIndex(this) + 1);
}

void CBullet::Update(DWORD dt)
{
	if (isDestroyed) return;
	x += vx * dt;
	y += vy * dt;
	if (x < 0 || x > CGame::GetInstance()->GetBackBufferWidth())
	{
		isDestroyed = true;
		return;
	}
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
	switch (orientation)
	{
	case 1:
		CGameObject::Render(downTexture);
		break;
	case 2:
		CGameObject::Render(leftTexture);
		break;
	case 3:
		CGameObject::Render(rightTexture);
		break;
	default:
		CGameObject::Render(upTexture);
		break;
	}
}

void CEnemy::Render()
{
	switch (health)
	{
	case 3:
		upTexture = redUpTexture;
		downTexture = redDownTexture;
		leftTexture = redLeftTexture;
		rightTexture = redRightTexture;
		break;
	case 2:
		upTexture = greenUpTexture;
		downTexture = greenDownTexture;
		leftTexture = greenLeftTexture;
		rightTexture = greenRightTexture;
		break;
	default:
		upTexture = whiteUpTexture;
		downTexture = whiteDownTexture;
		leftTexture = whiteLeftTexture;
		rightTexture = whiteRightTexture;
		break;
	}
	CTank::Render();
}

void CBullet::Render()
{
	if (isDestroyed) return;
	switch (orientation)
	{
	case 1:
		CGameObject::Render(downTexture);
		break;
	case 2:
		CGameObject::Render(leftTexture);
		break;
	case 3:
		CGameObject::Render(rightTexture);
		break;
	default:
		CGameObject::Render(upTexture);
		break;
	}
}
