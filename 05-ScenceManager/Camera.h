#pragma once

#include "Game.h"
class Camera
{
private:
	float cam_x;
	float cam_y;
	int width;
	int height;
	DWORD dt;
	CGame* Game;
public:
	Camera(int w, int h);
	~Camera();
	void SetGame(CGame* game) { Game = game; }
	CGame* GetGame() { return Game; }
	void SetPosition(float x, float y) { cam_x = x; cam_y = y; }
};
