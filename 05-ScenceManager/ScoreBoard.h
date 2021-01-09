#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Camera.h"
#include "Sprites.h"

#define RELATIVE_DY 192.0f

class CScoreBoard
{
	LPSPRITE sprite;
	float x;
	float y;
	DWORD dt;
public:
	CScoreBoard();

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }

	float GetPositionX() { return this->x; }
	float GetPositionY() { return this->y; }
	LPSPRITE GetSprite() { return this->sprite; }


	void Update(float cam_x,float cam_y);
	void Draw();

	~CScoreBoard();
};

