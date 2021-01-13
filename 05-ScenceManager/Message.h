#pragma once
#include "GameObject.h"

#define MESSAGE_STATE_MUSHROOM	0
#define MESSAGE_STATE_FLOWER	1
#define MESSAGE_STATE_STAR		2
class CMessage : public CGameObject
{	
public:
	CMessage();
	bool isDisplay;
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	~CMessage();
};
