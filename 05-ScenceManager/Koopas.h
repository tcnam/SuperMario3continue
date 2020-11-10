#pragma once

#include "GameObject.h"
#include "Mario.h"
#include "Brick.h"
#include "HiddenObject.h"
#include "BountyBrick.h"
#include "Goomba.h"

#define KOOPAS_WALKING_SPEED 0.03f;
#define KOOPAS_DYNAMIC_SPEED	0.2f
#define KOOPAS_GRAVITY			0.002f	
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_DEFENSE_STATIC	300
#define KOOPAS_STATE_DEFENSE_DYNAMIC	400

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
//#define KOOPAS_ANI_DIE 2
#define KOOPAS_ANI_DEFENSE_STATIC	2
#define KOOPAS_ANI_DEFENSE_DYNAMIC	3

class CKoopas : public CGameObject
{
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void left();
	void right();
	void walk();
	void stop();
	void dynamic();
	CKoopas();
	virtual void SetState(int state);
};