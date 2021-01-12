#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Camera.h"
#include "Sprites.h"
#include "Font.h"
#include "Mario.h"

#define RELATIVE_DX 172.0f
#define RELATIVE_DY 192.0f

class CSquare
{
	LPSPRITE sprite;
	float x;
	float y;

	CMario* Mario;

public:
	CSquare();

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }

	float GetPositionX() { return this->x; }
	float GetPositionY() { return this->y; }
	LPSPRITE GetSprite() { return this->sprite; }

	void SetMario(CMario* mario) { Mario = mario; }


	void Update(float cam_x, float cam_y);
	void Draw();

	~CSquare();
};


