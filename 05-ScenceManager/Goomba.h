#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "BountyBrick.h"

#define GOOMBA_WALKING_SPEED 0.05f;
#define GOOMBA_GRAVITY			0.002f	

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1

#define GOOMBA_UNTOUCHABLE_TIME		1000


class CGoomba : public CGameObject
{
public:
	CMario* Mario;
	bool untouchable;
	DWORD untouchable_start;
	CGoomba();
	void StartUntouchable() { untouchable = true; untouchable_start = (DWORD)GetTickCount64(); }
	void SetMario(CMario* mario) { Mario = mario; }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
};