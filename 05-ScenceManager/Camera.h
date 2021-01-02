#pragma once

#include "Game.h"
#include "Mario.h"
class Camera
{
public:
	/*static Camera* __instancecamera;*/
	float cam_x;
	float cam_y;
	CMario* Mario;
public:
	Camera();
	~Camera();
	void SetMario(CMario* mario) { Mario = mario; }
	void Update();
	//static Camera* GetInstance();
};
