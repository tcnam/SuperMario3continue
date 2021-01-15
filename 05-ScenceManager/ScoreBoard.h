#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Camera.h"
#include "Sprites.h"
#include "Font.h"
#include "Mario.h"

#define RELATIVE_DX_SB 10.0f
#define RELATIVE_DY_SB 192.0f

class CScoreBoard
{
	LPSPRITE sprite;
	float x;
	float y;
	DWORD dt;

	vector<CFont*> scores;
	vector<CFont*> time;
	vector<CFont*> numberOfCoins;	
	vector<CFont*> marioSpeedState;

	CFont* marioLifes;
	CFont* worldNumber;
	CFont* marioSpeedMaxState;
	CFont* backgroundFont;
	CMario* Mario;
	DWORD Mario_StartRunning;
	DWORD period;

	int temp_index;
public:
	
	CScoreBoard();

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }

	float GetPositionX() { return this->x; }
	float GetPositionY() { return this->y; }
	LPSPRITE GetSprite() { return this->sprite; }

	void PushScores(CFont* font) { scores.push_back(font); }
	void PushTime(CFont* font) { time.push_back(font); }
	void PushCoins(CFont* font) { numberOfCoins.push_back(font); }
	void PushSpeedState(CFont* font) { marioSpeedState.push_back(font); }

	void SetMarioLife(CFont* font) { marioLifes = font; }
	void SetWorldNumber(CFont* font) { worldNumber = font; }
	void SetMarioSpeedMaxState(CFont* font) { marioSpeedMaxState = font; }
	void SetBackGroundFont(CFont* font) { backgroundFont = font; }

	void SetMario(CMario* mario) { Mario = mario; }
	int FindSpeedindex(DWORD period);

	void ConvertNumber(int number, vector<CFont*> vectorFont);

	void Update(float cam_x,float cam_y);
	void Draw();

	~CScoreBoard();
};

