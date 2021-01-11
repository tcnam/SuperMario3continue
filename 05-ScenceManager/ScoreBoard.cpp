#include "ScoreBoard.h"
#include"Game.h"
#include"Utils.h"

CScoreBoard::CScoreBoard()
{
	x = 0;
	y = 0;
	dt = 0;
	sprite = NULL;
	marioLifes = NULL;
	marioSpeedMaxState = NULL;
	worldNumber = NULL;
}
void CScoreBoard::Update(float cam_x,float cam_y)
{
	x = cam_x;
	y = cam_y + RELATIVE_DY;
	for (unsigned int i = 0; i < scores.size(); i++)
	{
		scores[i]->Update(round(x),round(y));
	}
	for (unsigned int i = 0; i < numberOfCoins.size(); i++)
	{
		numberOfCoins[i]->Update(round(x), round(y));
	}
	for (unsigned int i = 0; i < time.size(); i++)
	{
		time[i]->Update(round(x), round(y));
	}
	for (unsigned int i = 0; i < marioSpeedState.size(); i++)
	{
		marioSpeedState[i]->Update(round(x), round(y));
	}
	worldNumber->Update(round(x),round(y));
	marioLifes->Update(round(x), round(y));
	marioSpeedMaxState->Update(round(x), round(y));
}
void CScoreBoard::Draw()
{
	CGame* game = CGame::GetInstance();
	game->Draw(round(x), round(y), sprite->GetText(), sprite->GetLeft(), sprite->GetTop(), sprite->GetRight(), sprite->GetBottom(), 255);
	for (unsigned int i = 0; i < scores.size(); i++)
	{
		scores[i]->Render();
	}
	for (unsigned int i = 0; i < numberOfCoins.size(); i++)
	{
		numberOfCoins[i]->Render();
	}
	for (unsigned int i = 0; i < time.size(); i++)
	{
		time[i]->Render();
	}
	for (unsigned int i = 0; i < marioSpeedState.size(); i++)
	{
		marioSpeedState[i]->Render();
	}
	worldNumber->Render();
	marioLifes->Render();
	marioSpeedMaxState->Render();
}
CScoreBoard::~CScoreBoard()
{

}