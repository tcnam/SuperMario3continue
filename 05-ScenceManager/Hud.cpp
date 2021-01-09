#pragma once
#include"Hud.h"
CHud::CHud()
{
	ScoreBoard = NULL;
	camera = NULL;
	cam_x = cam_y = 0.0f;
}
void CHud::Update()
{
	camera->GetPosition(cam_x, cam_y);
	ScoreBoard->Update(cam_x, cam_y);
}
void CHud::Render()
{
	ScoreBoard->Draw();
}
CHud::~CHud()
{

}