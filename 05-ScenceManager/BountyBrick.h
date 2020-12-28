#pragma once
#include "GameObject.h"
#include "Bounty.h"
#include "Mario.h"
#include "Koopas.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BOUNTYBRICK_STATE_NORMAL 0
#define BOUNTYBRICK_STATE_EMPTY	100

#define BOUNTYBRICK_MOVING_DISTANCE_TOBACK	4.00f
#define BOUNTYBRICK_SPEED_Y					0.1f

class CBountyBrick : public CGameObject
{
protected:
	float start_x;
	float start_y;
	CBounty* Bounty;
	CMario* Mario;
public:
	CBountyBrick();

	CBounty* GetBounty() { return Bounty; }
	CMario* GetMario() { return Mario; }

	void SetBounty(CBounty* bounty) { this->Bounty = bounty; }
	void SetMario(CMario* mario) { this->Mario = mario; }

	void SetStateBounty(int statebounty) { Bounty->SetState(statebounty); }
	void ActivateBounty() { Bounty->isUsed = true;  }
	void SetInitPosition(float x, float y) { start_x = x; start_y = y; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
	~CBountyBrick();
};