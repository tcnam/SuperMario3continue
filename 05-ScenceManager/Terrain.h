#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"


class CTerrain;
typedef CTerrain* LPTERRAIN;
class CTerrain
{
	LPSPRITE sprite;
	float x;
	float y;
public:	
	CTerrain();
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSprite(LPSPRITE sprite) { this->sprite=sprite; }
	float GetPositionX() { return this->x; }
	float GetPositionY() { return this->y; }
	LPSPRITE GetSprite() { return this->sprite; }
	void Draw(float x,float y,int alpha);
	~CTerrain();
};
