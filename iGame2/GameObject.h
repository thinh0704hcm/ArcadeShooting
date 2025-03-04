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
    LPTEXTURE texture;
public:
    void SetPosition(float x, float y) { this->x = x; this->y = y; }
    float GetX() { return x; }
    float GetY() { return y; }

    CGameObject(float x = 0.0f, float y = 0.0f, LPTEXTURE texture = NULL);

    virtual void Update(DWORD dt) = 0;
    virtual void Render();

    ~CGameObject();
};
typedef CGameObject* LPGAMEOBJECT;

class CBullet : public CGameObject
{
    float vx;
    float vy;
public:
    CBullet(float x, float y, float vx, float vy, LPTEXTURE texture) :CGameObject(x, y, texture)
    {
        this->vx = vx;
        this->vy = vy;
    };
    void Update(DWORD dt);
    void Render();
    bool isDestroyed = false;
    bool isVoid() { return vx && vy; }
};

class CShip : public CGameObject
{
    float vx;
    float vy;
    LPTEXTURE teamBulletTexture;

public:
    CShip(float x, float y, float vx, float vy, LPTEXTURE shipTexture, LPTEXTURE bulletTexture) :CGameObject(x, y, shipTexture)
    {
        this->vx = vx;
        this->vy = vy;
        this->teamBulletTexture = bulletTexture;
    };

    void SpawnBullet();
    void Update(DWORD dt);
    void Render();
    bool isDestroyed = false;
    bool CollisionCheck(float x, float y) {
        if (std::abs(x - this->GetX()) < 10 && std::abs(y - this->GetY()) < 10)
        {
            isDestroyed = true;
            return true;
        }
        return false;
    }
};

class CUFO : public CGameObject
{
    float vx;
    float vy;
	DWORD lastFireTime = 0;
    LPTEXTURE enemyBulletTexture;
public:
    CUFO(float x, float y, float vx, float vy, LPTEXTURE ufoTexture, LPTEXTURE bulletTexture) :CGameObject(x, y, ufoTexture)
    {
        this->vx = vx;
        this->vy = vy;
        this->enemyBulletTexture = bulletTexture;
    };

    void SpawnBullet();
    void Update(DWORD dt);
    void Render();
    bool isDestroyed = false;
    bool CollisionCheck(float x, float y) {
        if (std::abs(x - this->GetX()) < 10 && std::abs(y - this->GetY()) < 10)
        {
            isDestroyed = true;
            return true;
        }
        return false;
    }
};
