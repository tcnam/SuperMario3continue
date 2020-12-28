#pragma once
#include "GameObject.h"

#define FRAGMENT_BBOX_WIDTH  8
#define FRAGMENT_BBOX_HEIGHT 8

#define DISTANCE_DX_TO_DISAPPEAR	36.00f

#define FRAGMENT_SPEED_VX			0.1f
#define FRAGMENT_SPEED_VY			0.1f

#define FRAGMENT_SPEED_VX_2			0.005f
#define FRAGMENT_SPEED_VY_2			0.018f


class CFragment : public CGameObject
{
private:
	float start_x;
	float start_y;
public:
	bool isUsed;
	bool isFinished;
	CFragment();
	void SetInitPosition(float x, float y) { start_x = x; start_y = y; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~CFragment();
	//virtual void SetState(int state);
};