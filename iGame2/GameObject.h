#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <cmath>

#include "Texture.h"

class CGameObject
{
protected:
    float x;
    float y;

    // This should be a pointer to an object containing all graphic/sound/audio assets for rendering this object. 
    // For now, just a pointer to a single texture
    LPTEXTURE upTexture;
    LPTEXTURE downTexture;
    LPTEXTURE leftTexture;
    LPTEXTURE rightTexture;
public:
    void SetPosition(float x, float y) { this->x = x; this->y = y; }
    float GetX() { return x; }
    float GetY() { return y; }

    CGameObject(float x = 0.0f, float y = 0.0f, LPTEXTURE uptexture = NULL, LPTEXTURE downtexture = NULL, LPTEXTURE lefttexture = NULL, LPTEXTURE righttexture = NULL);

    virtual void Update(DWORD dt) = 0;
    virtual void Render(LPTEXTURE texture);

    ~CGameObject();
};
typedef CGameObject* LPGAMEOBJECT;

class CBullet : public CGameObject
{
    float vx = 50;
    float vy = 50;
public:
    CBullet(float x, float y, float vx, float vy, LPTEXTURE uptexture, LPTEXTURE downtexture, LPTEXTURE lefttexture, LPTEXTURE righttexture) :CGameObject(x, y, uptexture, downtexture, lefttexture, righttexture)
    {
        this->vx = vx;
        this->vy = vy;
    };
    void Update(DWORD dt);
    void Render();
    bool isDestroyed = false;
    bool isVoid() { return vx && vy; }
};

class CTank : public CGameObject
{
protected:
    float vx;
    float vy;
	LPTEXTURE bulletUpTexture;
	LPTEXTURE bulletDownTexture;
	LPTEXTURE bulletLeftTexture;
	LPTEXTURE bulletRightTexture;
    int health = 3;

public:
    CTank(float x, float y, float vx, float vy, LPTEXTURE upTexture, LPTEXTURE downTexture, LPTEXTURE leftTexture, LPTEXTURE rightTexture,
        LPTEXTURE bulletUp, LPTEXTURE bulletDown, LPTEXTURE bulletLeft, LPTEXTURE bulletRight) :CGameObject(x, y, upTexture, downTexture, leftTexture, rightTexture)
    {
        this->vx = vx;
        this->vy = vy;
		this->bulletUpTexture = bulletUp;
		this->bulletDownTexture = bulletDown;
		this->bulletLeftTexture = bulletLeft;
		this->bulletRightTexture = bulletRight;
    };

    void SpawnBullet();
    void Update(DWORD dt);
    void Render();
	int GetHealth() { return health; };
    bool isDestroyed() { return health == 0; };
    bool CollisionCheck(float x, float y) {
        if (std::abs(x - this->GetX()) < 7 && std::abs(y - this->GetY()) < 7)
        {
            health--;
            return true;
        }
        return false;
    }
};

class CEnemy : public CTank
{
	DWORD lastFireTime = 0;

	LPTEXTURE redUpTexture;
	LPTEXTURE redDownTexture;
	LPTEXTURE redLeftTexture;
	LPTEXTURE redRightTexture;

    LPTEXTURE greenUpTexture;
    LPTEXTURE greenDownTexture;
    LPTEXTURE greenLeftTexture;
    LPTEXTURE greenRightTexture;

    LPTEXTURE whiteUpTexture;
    LPTEXTURE whiteDownTexture;
    LPTEXTURE whiteLeftTexture;
    LPTEXTURE whiteRightTexture;
public:
    CEnemy(float x, float y, float vx, float vy, int health,
        LPTEXTURE redUpTexture, LPTEXTURE redDownTexture, LPTEXTURE redLeftTexture, LPTEXTURE redRightTexture,
        LPTEXTURE greenUpTexture, LPTEXTURE greenDownTexture, LPTEXTURE greenLeftTexture, LPTEXTURE greenRightTexture,
        LPTEXTURE whiteUpTexture, LPTEXTURE whiteDownTexture, LPTEXTURE whiteLeftTexture, LPTEXTURE whiteRightTexture, 
        LPTEXTURE bulletUp, LPTEXTURE bulletDown, LPTEXTURE bulletLeft, LPTEXTURE bulletRight )
		:CTank(x, y, vx, vy, upTexture, downTexture, leftTexture, rightTexture, bulletUp, bulletDown, bulletLeft, bulletRight)
	{
		this->redUpTexture = redUpTexture;
		this->redDownTexture = redDownTexture;
		this->redLeftTexture = redLeftTexture;
		this->redRightTexture = redRightTexture;
		this->greenUpTexture = greenUpTexture;
		this->greenDownTexture = greenDownTexture;
		this->greenLeftTexture = greenLeftTexture;
		this->greenRightTexture = greenRightTexture;
		this->whiteUpTexture = whiteUpTexture;
		this->whiteDownTexture = whiteDownTexture;
		this->whiteLeftTexture = whiteLeftTexture;
		this->whiteRightTexture = whiteRightTexture;
		this->bulletUpTexture = bulletUp;
		this->bulletDownTexture = bulletDown;
		this->bulletLeftTexture = bulletLeft;
		this->bulletRightTexture = bulletRight;
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
    };
    void SpawnBullet();
    void Update(DWORD dt);
};
