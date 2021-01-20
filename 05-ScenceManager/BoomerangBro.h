#pragma once
#include "GameObject.h"
#include "Mario.h"
#include"Boomerang.h"
#define DISTANCE_DX_TO_CHANGE_SPEED	16.0f

#define BOOMERANGBRO_STATE_HOLD		0
#define BOOMERANGBRO_STATE_NO_HOLD	1
#define BOOMERANGBRO_STATE_DIE		2

#define BOOMERANGBRO_VX				0.02f
#define BOOMERANG_GRAVITY			0.0008f	

#define BOOMERANGBRO_BBOX_WIDTH		16
#define BOOMERANGBRO_BBOX_HEIGHT	24

class CBoomerangBro : public CGameObject
{
	float start_x;
	float start_y;
	CMario* Mario;
	CBoomerang* Boomerang;
public:
	CBoomerangBro();
	void SetBoomerang(CBoomerang* b) { Boomerang = b; }
	void SetMario(CMario* mario) { Mario = mario; }
	void ThrowBoomerang();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetInitPosition(float x, float y) { start_x = x; start_y = y; }
	~CBoomerangBro();
};