#pragma once
#include"GameObject.h"
class CHiddenObject :public CGameObject
{
	int width;
	int height;
public:
	CHiddenObject(float x, float y, int width, int height);
	~CHiddenObject();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};