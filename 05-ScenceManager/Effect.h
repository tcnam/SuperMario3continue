#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include"Game.h"
#include"Utils.h"
#include "Sprites.h"
#include "Animations.h"

#define EFFECT_CLOUND		0
#define EFFECT_SMALL_BIG	1

#define EFFECT_OWNER_ID_MARIO		10
#define EFFECT_OWNER_ID_FIREFLOWER	20
//#define EFFECT_OWNER_ID_TAIL		30

#define EFFECT_LOCATION_X_TO_HIDE	48.0f
#define EFFECT_LOCATION_Y_TO_HIDE	48.0f
using namespace std;

class CEffect
{
	LPANIMATION_SET animation_set;
	float x;
	float y;
	int state;
	int owner_id;
	DWORD dt;
	
public:
	bool RightOrLeft;
	CEffect();
	void SetOwnerId(int n) { owner_id = n; }
	int GetOwnerId() { return owner_id; }

	void SetPosition(float lo_x, float lo_y) { x = lo_x, y = lo_y; }
	float GetPositionX() { x; }
	float GetPositionY() { y; }

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	void Update(DWORD dt);
	void Render();
	void SetState(int state) { this->state = state; }

	~CEffect();
};

