#pragma once

#include "Game.h"
#include "Mario.h"
#define CAM_X_MAX_WHEN_Y_NEGATIVE	2560.0f
#define CAM_X_MIN_WHEN_UNDERGROUND	2128.0f
#define CAM_X_MAX_WHEN_UNDERGROUND	2384.0f
#define CAM_Y_WHEN_UNDERGROUND		64.0f
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
