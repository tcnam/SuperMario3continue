#pragma once
#include"GameObject.h"
#include"Mario.h"
class CHiddenDoor :public CGameObject
{
	int width;
	int height;
	CMario* Mario;
public:
	bool UpOrDown;
	CHiddenDoor(float x, float y, int width, int height);
	~CHiddenDoor();
	void SetMario(CMario* mario) { Mario = mario; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
