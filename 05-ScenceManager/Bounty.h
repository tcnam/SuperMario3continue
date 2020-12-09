#pragma once
#include "GameObject.h"

#define BOUNTY_BBOX_WIDTH  16
#define BOUNTY_BBOX_HEIGHT 16

#define BOUNTY_STATE_COIN		0
#define BOUNTY_STATE_POWERUP	1
#define BOUNTY_STATE_LIFEUP		2

#define BOUNTY_COIN_VY		0.15f
#define BOUNTY_OTHERS_VY	0.015f
#define BOUNTY_OTHERS_VX	0.015f
#define BOUNTY_GRAVITY		0.01f

#define BOUNTY_COIN_ANI				0
#define BOUNTY_RED_MUSHROOM_ANI		1
#define BOUNTY_LEAF_ANI				2
#define BOUNTY_GREEN_MUSHROOM_ANI	3
class CBounty : public CGameObject
{
public:
	bool isLeaf;
	bool isUsed;
	CBounty();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	~CBounty();
};
