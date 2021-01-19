#pragma once
#include "GameObject.h"
#include "Game.h"
#define FLYBRICK_BBOX_WIDTH  48
#define FLYBRICK_BBOX_HEIGHT 16

#define FLYBRICK_STATE_VX	0
#define FLYBRICK_STATE_VY	1

#define FLYBRICK_VX			0.03f
#define FLYBRICK_GRAVITY	0.00015f
class CFlyBrick : public CGameObject
{
	bool UpdateOrNot;
public:
	CFlyBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects=NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	~CFlyBrick();
};