#pragma once
#include "GameObject.h"

#define DISTANCE_DX_TO_CHANGE_SPEED	16.0f

#define BOOMERANGBRO_STATE_HOLD		0
#define BOOMERANGBRO_STATE_NO_HOLD	1
#define BOOMERANGBRO_STATE_DIE		2

#define BOOMERANGBRO_VX				0.01f
#define BOOMERANG_GRAVITY			0.0008f	
class CBoomerangBro : public CGameObject
{
	float start_x;
	float start_y;
public:
	CBoomerangBro();
	/*virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);*/
	void SetInitPosition(float x, float y) { start_x = x; start_y = y; }
	~CBoomerangBro();
};