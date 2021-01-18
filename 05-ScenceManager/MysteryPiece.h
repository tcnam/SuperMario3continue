#pragma once
#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "Game.h"
#include "Message.h"

#define MYSTERYPIECE_BBOX_WIDTH  16
#define MYSTERYPIECE_BBOX_HEIGHT 16


#define MYSTERYPIECE_STATE_MUSHROOM	0
#define MYSTERYPIECE_STATE_FLOWER	1
#define MYSTERYPIECE_STATE_STAR		2

#define MYSTERYPIECE_FLY_SPEED		0.15f
#define MYSTERYPIECE_Y_LIMIT		208.0f

#define MESSAGE_RELATIVE_X			48.0f
#define MESSAGE_RELATIVE_Y			16.0f

class CMysteryPiece : public CGameObject
{
	CMario* Mario;
	CMessage* message;
public:
	bool isCollidedWithMario;
	bool isFinished;	
	DWORD t;				//time count for changing state;
	CMysteryPiece();
	void SetMessage(CMessage* ms) { message = ms; }
	void SetMario(CMario* mario) { Mario = mario; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	~CMysteryPiece();
};