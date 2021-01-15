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
	Mario_StartRunning = 0;
	period = 0;
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
	float Mario_vx, Mario_vy;
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
	Mario->GetSpeed(Mario_vx, Mario_vy);
	if (Mario->isFlying == false)
	{
		if (abs(Mario_vx) == MARIO_RUNNING_SPEED && Mario->isOnGround == true)
		{
			if (Mario_StartRunning == 0)
			{
				Mario_StartRunning = (DWORD)GetTickCount64();
			}
			if (Mario_StartRunning != 0)
			{
				
				/*period = (DWORD)GetTickCount64() - Mario_StartRunning;
				int temp_index = FindSpeedindex(period);*/
				int temp_index = 3;
				for (int i = 0; i < temp_index; i++)
				{
					marioSpeedState[i]->SetState(FONT_STATE_WHITE);
				}
			}
				
			marioSpeedMaxState->SetState(FONT_STATE_MAX_BLACK);
		}
		else if (abs(Mario_vx) == MARIO_RUNNINGFAST_SPEED && Mario->isOnGround == true)
		{
			Mario_StartRunning = 0;
			period = 0;
			for (unsigned int i = 0; i < marioSpeedState.size(); i++)
			{
				marioSpeedState[i]->SetState(FONT_STATE_WHITE);
			}
			marioSpeedMaxState->SetState(FONT_STATE_MAX_WHITE);
		}
		else
		{
			period = 0;
			Mario_StartRunning = 0;
			for (unsigned int i = 0; i < marioSpeedState.size(); i++)
			{
				marioSpeedState[i]->SetState(FONT_STATE_BLACK);
			}
			marioSpeedMaxState->SetState(FONT_STATE_MAX_BLACK);
		}
	}
	else
	{
		for (unsigned int i = 0; i < marioSpeedState.size(); i++)
		{
			marioSpeedState[i]->SetState(FONT_STATE_WHITE);
		}
		marioSpeedMaxState->SetState(FONT_STATE_MAX_WHITE);
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
int CScoreBoard::FindSpeedindex(DWORD period)
{
	int temp_index;
	if (period < 200 && period>0)
	{
		temp_index = 1;
	}
	else if (period > 200 && period < 400)
	{
		temp_index = 2;
	}
	else if (period > 400 && period < 600)
	{
		temp_index = 3;
	}
	else if (period > 600 && period < 800)
	{
		temp_index = 4;
	}
	else if (period > 800 && period < 1000)
	{
		temp_index = 5;
	}
	else if (period > 1200)
	{
		temp_index = 6;
	}
	else
		temp_index = 3;

	return temp_index;
}
CScoreBoard::~CScoreBoard()
{

}