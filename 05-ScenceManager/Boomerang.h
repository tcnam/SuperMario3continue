#pragma once
#pragma once
#include"GameObject.h"
#include"Mario.h"
#define FIREBALLFLOWER_GRAVITY		0.0005f
#define BOOMERANG_SPEED_X		0.04f
#define BOOMERANG_SPEED_Y		0.04f
#define BOOMERANG_BBOX_WIDTH		16
#define BOOMERANG_BBOX_HEIGHT		16
#define DISTANCE_DX_TOCHANGE_VX		144.0f
class CBoomerang :public CGameObject
{
	float start_x;
	float start_y;
	CMario* Mario;
public:
	bool isUsed;
	bool isFinish;
	CBoomerang();
	void SetMario(CMario* mario) { Mario = mario; }
	void SetInitPositon(float x, float y) { start_x = x, start_y = y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	~CBoomerang();
};