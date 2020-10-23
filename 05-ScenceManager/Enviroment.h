#pragma once
#include "GameObject.h"

#define ENVIROMENT_BBOX_WIDTH 16 
#define ENVIROMENT_BBOX_HEIGHT 16

class CEnviroment : public CGameObject
{
public:
	virtual void Render();
	//virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};