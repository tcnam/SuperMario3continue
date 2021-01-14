#pragma once
#include "GameObject.h"
#include "Coin.h"
#include "Mario.h"
#include "Koopas.h"
#include"Fragment.h"

#define WEAKBRICK_BBOX_WIDTH  16
#define WEAKBRICK_BBOX_HEIGHT 16

#define WEAKBRICK_STATE_NORMAL		0
#define WEAKBRICK_STATE_DISAPPEAR	100

#define WEAKBRICK_DY_TO_MOVE		640.00f


class CWeakBrick : public CGameObject
{
protected:
	float start_x;
	float start_y;
	CCoin* Coin;
	vector<CFragment*> fragments;
public:
	CWeakBrick();


	void PushCoin(CCoin* coin) { this->Coin= coin; }
	CCoin* GetCoin() { return Coin; }
	
	void PushFragment(CFragment* fragment) { fragments.push_back(fragment); }
	void ActivateFragment();
	void SetInitPosition(float x, float y) { start_x = x; start_y = y; }
	void MoveWeakBrickToHorizon() { SetPosition(start_x, start_y + WEAKBRICK_DY_TO_MOVE); }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	~CWeakBrick();
};