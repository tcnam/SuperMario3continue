#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "BountyBrick.h"
#include "Game.h"

#define GOOMBA_WALKING_SPEED 0.05f;
#define GOOMBA_GRAVITY			0.0008f	
#define	GOOMBA_FLY_JUMP_SPEED	0.24f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 16

#define GOOMBA_FLY_JUMP_BBOX_WIDTH	20
#define GOOMBA_FLY_JUMP_BBOX_HEIGHT	24
#define GOOMBA_FLY_BBOX_WIDTH		20
#define GOOMBA_FLY_BBOX_HEIGHT		19
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_LEVEL_NORMAL		0
#define GOOMBA_LEVEL_FLY		1

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200
#define GOOMBA_STATE_KICKED_OUT	300
//#define GOOMBA_STATE_JUMP		400

#define GOOMBA_ANI_WALKING 0
#define GOOMBA_ANI_DIE 1
#define GOOMBA_ANI_KICKOUT	2
#define GOOMBA_FLY_ANI_WALKING	3
#define GOOMBA_FLY_ANI_JUMP		4

#define GOOMBA_UNTOUCHABLE_TIME		300

#define GOOMBA_DX_LIMIT_TOFLY		64.0f

class CGoomba : public CGameObject
{
	float start_x;
	float start_y;
	int level;
public:
	CMario* Mario;
	bool untouchable;
	DWORD untouchable_start;
	CGoomba();
	void StartUntouchable() { untouchable = true; untouchable_start = (DWORD)GetTickCount64(); }
	void SetMario(CMario* mario) { Mario = mario; }

	void Setlevel(int l) { level = l; }
	int GetLevel() { return level; }
	void SetInitPosition(float x, float y) { start_x = x; start_y = y; }

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	~CGoomba();
};