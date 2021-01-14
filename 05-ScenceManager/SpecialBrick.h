#pragma once
#include "GameObject.h"
#include "Koopas.h"
#include "BountyButton.h"
#include"Fragment.h"

#define SPECIALBRICK_BBOX_WIDTH  16
#define SPECIALBRICK_BBOX_HEIGHT 16

#define SPECIALBRICK_STATE_NORMAL		0
#define SPECIALBRICK_STATE_EMPTY		100


class CSpecialBrick : public CGameObject
{
protected:
	float start_x;
	float start_y;
	CBountyButton* bountybutton;
	vector<CFragment*> fragments;

public:
	CSpecialBrick();

	void SetBountyButton(CBountyButton* bb) { bountybutton = bb; }
	void PushFragment(CFragment* fragment) { fragments.push_back(fragment); }

	void ActivateBountyButton();
	void ActivateFragment();
	void SetInitPosition(float x, float y) { start_x = x; start_y = y; }

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	~CSpecialBrick();
};