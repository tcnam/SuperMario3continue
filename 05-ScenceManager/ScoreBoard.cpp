#include "ScoreBoard.h"
#include"Game.h"
#include"Utils.h"

CScoreBoard::CScoreBoard()
{
	x = 0;
	y = 0;
	dt = 0;
	sprite = NULL;
}
void CScoreBoard::Update(float cam_x,float cam_y)
{
	x = cam_x;
	y = cam_y + RELATIVE_DY;
}
void CScoreBoard::Draw()
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, sprite->GetText(), sprite->GetLeft(), sprite->GetTop(), sprite->GetRight(), sprite->GetBottom(), 255);
}
CScoreBoard::~CScoreBoard()
{

}