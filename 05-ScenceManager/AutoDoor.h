#pragma once
#include"GameObject.h"
#include"Mario.h"
#include"Game.h"
class CAutoDoor :public CGameObject
{
	int width;
	int height;
	float destination_x;
	float destination_y;
	CMario* Mario;
public:
	CAutoDoor(float x, float y, int width, int height,float des_x, float des_y);
	~CAutoDoor();
	void SetMario(CMario* mario) { Mario = mario; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
