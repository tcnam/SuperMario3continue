#pragma once
#include"ScoreBoard.h"
#include "Camera.h"
#include "Square.h"

#define HUD_TYPE_SCOREBOARD		0
#define HUD_TYPE_SQUARE			1
#define HUD_TYPE_FONT			2
#define HUD_TYPE_BACKGROUND		3

class CHud
{
	float cam_x;
	float cam_y;
	LPSPRITE background;
	CScoreBoard* ScoreBoard;
	CSquare* Square;
	Camera* camera;
public:
	CHud();

	LPSPRITE GetSprite() { return this->background; }
	void SetSprite(LPSPRITE sp) { background = sp; }

	void SetCamera(Camera* cm) { camera = cm; }

	void SetScoreBoard(CScoreBoard* sb) { ScoreBoard = sb; }
	CScoreBoard* GetScoreBoard() { return ScoreBoard; }

	void SetSquare(CSquare* sq) { Square = sq; }
	CSquare* GetSquare() { return Square; }

	void Update();
	void Render();
	~CHud();
};
