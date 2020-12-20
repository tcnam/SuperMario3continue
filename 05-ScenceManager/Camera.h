#pragma once

#include "Game.h"
class Camera
{
public:
	/*static Camera* __instancecamera;*/
	float cam_x;
	float cam_y;
	
public:
	Camera();
	~Camera();

	void SetPosition(float x, float y) { cam_x = x; cam_y = y; }
	//static Camera* GetInstance();
};
