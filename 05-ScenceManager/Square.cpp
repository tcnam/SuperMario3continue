#include "Square.h"
#include"Game.h"
#include"Utils.h"

CSquare::CSquare()
{
	x = 0;
	y = 0;
	Mario = NULL;
}
void CSquare::Update(float cam_x, float cam_y)
{
	x = cam_x+  RELATIVE_DX;
	y = cam_y + RELATIVE_DY;
}
void CSquare::Draw()
{
	CGame* game = CGame::GetInstance();
	game->Draw(round(x), round(y), sprite->GetText(), sprite->GetLeft(), sprite->GetTop(), sprite->GetRight(), sprite->GetBottom(), 255);
}
CSquare::~CSquare()
{

}