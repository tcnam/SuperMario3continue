#pragma once
#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Game.h"

#define MYSTERYPIECE_BBOX_WIDTH  16
#define MYSTERYPIECE_BBOX_HEIGHT 16


#define MYSTERYPIECE_STATE_MUSHROOM	0
#define MYSTERYPIECE_STATE_FLOWER	1
#define MYSTERYPIECE_STATE_STAR		2

class CMysteryPiece : public CGameObject
{
public:
	bool isFinished;
	CMario* Mario;
	DWORD t;
	CMysteryPiece();
	void SetMario(CMario* mario) { Mario = mario; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~CMysteryPiece();
};