#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include "Camera.h"
#include "Sprites.h"
#include "Font.h"
#include "Mario.h"
#include "MysteryPiece.h"
#include "Game.h"

#define RELATIVE_DX 172.0f
#define RELATIVE_DY 192.0f

#define RELATIVE_DX_MYSTERYPIECE_1	5.0f
#define RELATIVE_DX_MYSTERYPIECE_2	29.0f
#define RELATIVE_DX_MYSTERYPIECE_3	53.0f

#define RELATIVE_DY_MYSTERYPIECE	7.0f;
class CSquare
{
	LPSPRITE sprite;
	float x;
	float y;
	vector<CMysteryPiece*> Pieces;
	CMario* Mario;

public:
	CSquare();

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSprite(LPSPRITE sprite) { this->sprite = sprite; }

	float GetPositionX() { return this->x; }
	float GetPositionY() { return this->y; }
	LPSPRITE GetSprite() { return this->sprite; }

	void SetMario(CMario* mario) { Mario = mario; }

	void PushVectorPieces(CMysteryPiece* piece) { Pieces.push_back(piece); }
	void PushMysteryPiece(CMysteryPiece* piece);

	void Update(float cam_x, float cam_y);
	void Draw();

	~CSquare();
};


