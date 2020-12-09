#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH  16
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject
{
public:
	bool isFinished;
	CCoin();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~CCoin();
};
