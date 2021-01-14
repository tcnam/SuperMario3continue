#pragma once
#include "GameObject.h"
#include "FireBallFLower.h"
#include "Mario.h"

#define FIREFLOWER_FLY_SPEED 0.015f;	

#define FIREFLOWER_BBOX_WIDTH 16
#define FIREFLOWER_BBOX_HEIGHT 32

#define FIREFLOWER_STATE_RIGHT_UPPER	100
#define FIREFLOWER_STATE_RIGHT_LOWER	200
#define FIREFLOWER_STATE_LEFT_UPPER		300
#define FIREFLOWER_STATE_LEFT_LOWER		400

#define FIREFLOWER_ANI_RIGHT_UPPER	0
#define FIREFLOWER_ANI_RIGHT_LOWER	1
#define FIREFLOWER_ANI_LEFT_UPPER	2
#define FIREFLOWER_ANI_LEFT_LOWER	3

#define FIREFLOWER_UPPER_Y			-96
#define FIREFLOWER_LOWER_Y			-48
#define FIREFLOWR_APPEAR_Y			-64

#define FIREFLOWER_TIMEWAIT_FORSHOOT	1000




class CFireFlower : public CGameObject
{
public:
	CMario* Mario;
	bool isShooting;
	bool isShootFar;
	bool isAppear;
	DWORD Shoot_Start;

	CFireFlower();
	CFireBallFlower* FireBallFlower;
	void StartShoot() { isShooting = true; Shoot_Start = (DWORD)GetTickCount64(); vy = 0; }
	void Shoot();
	void SetMario(CMario* mario) { Mario = mario; }
	void SetFireBallFlower(CFireBallFlower* fireball) { FireBallFlower = fireball; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	~CFireFlower();
};