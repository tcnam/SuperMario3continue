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
	backgroundFont = NULL;
	Mario = NULL;
}
void CScoreBoard::ConvertNumber(int n,vector<CFont*> vectorFont)
{
	vector<int> temp;
	while (n != 0)
	{
		temp.push_back(n % 10);
		n = n / 10;
	}
	if (temp.size() == 1)
	{
		vectorFont[1]->SetState(temp[0]);
		return;
	}
	else
	{
		int indexVector = vectorFont.size() - 1;
		for (unsigned int i = 0; i <temp.size(); i++)
		{			
			vectorFont[indexVector]->SetState(temp[i]);
			indexVector--;
		}
	}	
}
void CScoreBoard::Update(float cam_x,float cam_y)
{
	if (Mario == NULL)
		return;
	x = cam_x+RELATIVE_DX_SB;
	y = cam_y + RELATIVE_DY_SB;
	for (unsigned int i = 0; i < scores.size(); i++)
	{
		ConvertNumber(CGame::GetInstance()->GetScores(), scores);
		scores[i]->Update(x,y);
	}
	for (unsigned int i = 0; i < numberOfCoins.size(); i++)
	{
		ConvertNumber(CGame::GetInstance()->GetCoins(),numberOfCoins);
		numberOfCoins[i]->Update(x, y);
	}
	for (unsigned int i = 0; i < time.size(); i++)
	{
		ConvertNumber(CGame::GetInstance()->GetTime(), time);
		time[i]->Update(x, y);
	}
	for (unsigned int i = 0; i < marioSpeedState.size(); i++)
	{
		marioSpeedState[i]->Update(x, y);
	}
	backgroundFont->Update(x, y);

	worldNumber->SetState(CGame::GetInstance()->GetWorld());
	worldNumber->Update(x,y);

	marioLifes->SetState(CGame::GetInstance()->GetLife());
	marioLifes->Update(x, y);

	marioSpeedMaxState->Update(x, y);
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
	backgroundFont->Render();
	worldNumber->Render();
	marioLifes->Render();
	marioSpeedMaxState->Render();
}
CScoreBoard::~CScoreBoard()
{

}