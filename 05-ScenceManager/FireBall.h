#pragma once
#include"GameObject.h"
#define FIREBALL_SPEED		0.05f
#define FIREBALL_GRAVITY	0.05f

#define FIREBALL_BBOX_WIDTH		8
#define FIREBALL_BBOX_HEIGHT	8
class CFireBall :public CGameObject
{	
public:
	bool isFinished;
	bool isUsed;
	CFireBall();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	void Attack(float x, float y, int Direction);	
	~CFireBall();
};
