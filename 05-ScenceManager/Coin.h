#pragma once
#include "GameObject.h"
#include "Mario.h"

#define COIN_BBOX_WIDTH  16
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject
{
public:
	bool isFinished;
	CMario* Mario;
	
	CCoin();
	void SetMario(CMario* mario) { Mario = mario; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~CCoin();
};
