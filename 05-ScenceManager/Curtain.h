#pragma once
#include "GameObject.h"
#define Y_LIMIT			-446.0f
#define CURTAIN_FLY_SPEED		0.1f
class CCurtain : public CGameObject
{
public:
	CCurtain();
	~CCurtain();
	bool isFinish;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
