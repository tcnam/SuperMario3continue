#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "WeakBrick.h"

#define BOUNTYBUTTON_BBOX_WIDTH  16
#define BOUNTYBUTTON_BBOX_HEIGHT 16
#define BOUNTYBUTTON_BBOX_HEIGHT_ISPRESSED	7

#define BOUNTYBUTTON_TIME_WAIT				8000

class CBountyButton : public CGameObject
{
	CMario* Mario;
	vector<CWeakBrick*> weakbricks;
	DWORD Bounty_start;
public:
	bool isFinished;
	bool isInsideSpecialBrick;
	CBountyButton();
	void SetMario(CMario* mario) { Mario = mario; }
	void PushWeakBrick(CWeakBrick* weakbirck) { weakbricks.push_back(weakbirck); }
	void ActivateWeakBrick();
	void BringBackWeakBrick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~CBountyButton();
};
