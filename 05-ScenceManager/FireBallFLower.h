#pragma once
#include"GameObject.h"
#define FIREBALLFLOWER_SPEED		0.05f
#define FIREBALLFLOWER_GRAVITY	0.0005f
#define FIREBALLFLOWER_SPEED_X	0.04f
#define FIREBALLFLOWER_SPEED_Y	0.04f
#define FIREBALLFLOWER_SPEED_XFAR	sqrt(6)/50.0f
#define FIREBALLFLOWER_SPEED_YFAR	sqrt(2)/50.0f
#define FIREBALLFLOWER_BBOX_WIDTH		8
#define FIREBALLFLOWER_BBOX_HEIGHT		8
class CFireBallFlower :public CGameObject
{
public:
	bool isFinished;
	bool isUsed;
	CFireBallFlower();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Attack(float x, float y, int Direction);
	~CFireBallFlower();
};
