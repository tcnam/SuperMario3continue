#pragma once
#include "GameObject.h"
#include "Bounty.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BOUNTYBRICK_STATE_NORMAL 0
#define BOUNTYBRICK_STATE_EMPTY	100

class CBountyBrick : public CGameObject
{
public:
	
	CBountyBrick();
	CBounty* Bounty;
	void SetBounty(CBounty* bounty) { this->Bounty = bounty; }
	//virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};