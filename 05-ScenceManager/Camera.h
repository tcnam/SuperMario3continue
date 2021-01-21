#pragma once
#include "Utils.h"
#include "Game.h"
#include "Mario.h"
#define CAM_X_MAX_WHEN_Y_NEGATIVE	2560.0f
#define CAM_X_MIN_WHEN_UNDERGROUND	2128.0f
#define CAM_X_MAX_WHEN_UNDERGROUND	2384.0f
#define CAM_Y_WHEN_UNDERGROUND		64.0f

#define CAM_X_IN_USE				272.0f
#define CAM_Y_IN_USE				192.0f

#define BONUS_SCENCE_1_1			48.0f
#define CAM_SPEED_VX_SCENCE_1_4		0.04f
class Camera
{
	float cam_x;
	float cam_y;
	float vx; 
	float vy;
	DWORD dt;
	CMario* Mario;
public:
	Camera();
	~Camera();
	void SetPosition(float x, float y) { this->cam_x = x, this->cam_y = y; }
	void GetPosition(float& x, float& y) { x = this->cam_x; y = this->cam_y; }
	bool checkifMarioInside();
	bool CheckIfObjectInside(float x, float y);
	void SetMario(CMario* mario) { Mario = mario; }
	void Update(DWORD dt);

};
