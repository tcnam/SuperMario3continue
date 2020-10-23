#pragma once
#include "GameObject.h"

#define ENVIR_BBOX_WIDTH  16
#define ENVIR_BBOX_HEIGHT 16

class CEnviroment : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};