#pragma once
#include"ScoreBoard.h"
#include "Camera.h"

#define HUD_TYPE_SCOREBOARD		0
#define HUD_TYPE_SQUARE			1
#define HUD_TYPE_FONT			2

class CHud
{
	float cam_x;
	float cam_y;
	CScoreBoard* ScoreBoard;
	Camera* camera;
public:
	CHud();
	void SetCamera(Camera* cm) { camera = cm; }
	void SetScoreBoard(CScoreBoard* sb) { ScoreBoard = sb; }
	void Update();
	void Render();
	~CHud();
};
