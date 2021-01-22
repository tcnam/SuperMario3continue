#pragma once
#include "GameObject.h"
#include "Utils.h"
#include "Mario.h"
#include "Game.h"

#define BOUNTY_BBOX_WIDTH  16
#define BOUNTY_BBOX_HEIGHT 16
#define BOUNTY_COIN_BBOX_WIDTH	8
#define BOUNTY_COIN_BBOX_HEIGHT	16

#define BOUNTY_STATE_COIN		0
#define BOUNTY_STATE_POWERUP	1
#define BOUNTY_STATE_LIFEUP		2

#define BOUNTY_COIN_VY		0.3f
#define BOUNTY_MUSHROOM_VY	0.05f
#define BOUNTY_MUSHROOM_VX	0.05f
#define BOUNTY_LEAF_VY		0.1f
#define BOUNTY_LEAF_VX		0.01f
#define BOUNTY_GRAVITY		0.015f

#define BOUNTY_MUSHROOM_DISTANCE_DY_TOHAVE_VX	17.00f
#define BOUNTY_LEAF_DISTANCE_DY_TOFALL			48.00f
#define BOUNTY_LEAF_DISTANCE_DX_TOCHANGE_VX		32.00f
#define BOUNTY_COIN_DISTANCE_DY_TOFALL			64.00f

#define BOUNTY_COIN_ANI				0
#define BOUNTY_RED_MUSHROOM_ANI		1
#define BOUNTY_LEAF_ANI				2
#define BOUNTY_GREEN_MUSHROOM_ANI	3
#define BOUNTY_FIRE_FLOWER_ANI		4

#define BOUNTY_ACTIVE_TIME			300
class CBounty : public CGameObject
{
private:
	float start_x;
	float start_y;
public:
	CMario* Mario;
	int powerindex;						//determide powerup is Leaf or red mushroom
	bool isUsed;						//determide if bounty was activated by bounty brick
	bool isCrossBoundary;				//determide if bounty crossed its limit boundary
	bool isFinised;						//determide if bounty was finished or not
	bool isRightDirection;				//determide bounty powerup will move to right or left, true:right, false:left
	CBounty();
	void SetInitPosition(float x, float y) { start_x = x; start_y = y; }
	void SetMario(CMario* mario) { Mario = mario; }
	CMario* GetMario() { return Mario; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	~CBounty();
};
