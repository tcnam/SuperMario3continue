#pragma once

#include "Game.h"
#include "Mario.h"
class Camera
{
	float cam_x;
	float cam_y;
	CMario* Mario;
public:
	Camera();
	~Camera();
	void SetPosition(float x, float y) { this->cam_x = x, this->cam_y = y; }
	void GetPosition(float& x, float& y) { x = this->cam_x; y = this->cam_y; }
	void SetMario(CMario* mario) { Mario = mario; }
	void Update();

};
