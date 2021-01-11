#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Camera.h"
#include "Sprites.h"
#include "Font.h"

#define RELATIVE_DY 192.0f

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

	void Update(float cam_x,float cam_y);
	void Draw();

	~CScoreBoard();
};

