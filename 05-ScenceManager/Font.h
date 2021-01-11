#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"
//#include "ScoreBoard.h"

using namespace std;

#define FONT_TYPE_BACKGROUND	0
#define FONT_TYPE_NUMBER		1
#define FONT_TYPE_STATE			2
#define FONT_TYPE_STATE_MAX		3

#define FONT_NUMBER_0		0
#define FONT_NUMBER_1		1
#define FONT_NUMBER_2		2
#define FONT_NUMBER_3		3
#define FONT_NUMBER_4		4
#define FONT_NUMBER_5		5
#define FONT_NUMBER_6		6
#define FONT_NUMBER_7		7
#define FONT_NUMBER_8		8
#define FONT_NUMBER_9		9

#define FONT_STATE_BLACK	10
#define FONT_STATE_WHITE	11

#define FONT_STATE_MAX_BLACK	12
#define FONT_STATE_MAX_WHITE	13

class CFont
{
	float x;
	float y;
	float relative_x;
	float relative_y;
	int type;
	int state;
	LPANIMATION_SET animation_set;
public:
	CFont();
	~CFont();
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	void SetRelativePosition(float x, float y) { relative_x = x; relative_y = y; }
	void Update(float ScoreBoard_x,float ScoreBoard_y);
	void Render();
	void SetState(int s) { state = s; }
	void SetType(int t) { type = t; }
};

