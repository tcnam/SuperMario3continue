#pragma once
#include"Hud.h"
CHud::CHud()
{
	Square = NULL;
	background = NULL;
	ScoreBoard = NULL;
	camera = NULL;
	cam_x = cam_y = 0.0f;
}
void CHud::Update()
{
	camera->GetPosition(cam_x, cam_y);
	ScoreBoard->Update(cam_x, cam_y);
	Square->Update(cam_x, cam_y);
}
void CHud::Render()
{
	CGame* game = CGame::GetInstance();
	game->Draw(round(cam_x), round(cam_y+RELATIVE_DY), background->GetText(), background->GetLeft(), background->GetTop(), background->GetRight(), background->GetBottom(), 255);
	ScoreBoard->Draw();
	Square->Draw();
}
CHud::~CHud()
{

}