#include "Square.h"
#include"Game.h"
#include"Utils.h"

CSquare::CSquare()
{
	x = 0;
	y = 0;
	Mario = NULL;
	sprite = NULL;
}
void CSquare::PushMysteryPiece(CMysteryPiece* p)
{
	for (unsigned int i = 0; i < Pieces.size(); i++)
	{
		if (Pieces[i]->GetState()==MYSTERYPIECE_STATE_MUSHROOM)
		{
			Pieces[i] = p;
			break;
		}			
	}
}
void CSquare::Update(float cam_x, float cam_y)
{
	x = cam_x+  RELATIVE_DX;
	y = cam_y + RELATIVE_DY;
	int temp1 = CGame::GetInstance()->GetPiece1();
	int temp2 = CGame::GetInstance()->GetPiece2();
	int temp3 = CGame::GetInstance()->GetPiece3();
	for (unsigned int i = 0; i < Pieces.size(); i++)
	{
		if (Pieces[i] == NULL)
			return;
		if (Pieces[i]->isFinished == false)
			return;
		//CGame::GetInstance()->SetStatesOfMyPieces(Pieces[i]->GetState());
		float piece_x, piece_y;
		if (i == 0)
		{
			piece_x = x + RELATIVE_DX_MYSTERYPIECE_1;
			piece_y = y + RELATIVE_DY_MYSTERYPIECE;
		}
			
		else if (i == 1)
		{
			piece_x = x + RELATIVE_DX_MYSTERYPIECE_2;
			piece_y = y + RELATIVE_DY_MYSTERYPIECE;
		}
		else
		{
			piece_x = x + RELATIVE_DX_MYSTERYPIECE_3;
			piece_y = y + RELATIVE_DY_MYSTERYPIECE;
		}
		Pieces[i]->SetPosition(piece_x,piece_y);
		if(i==0)
			Pieces[i]->SetState(temp1);
		else if(i==1)
			Pieces[i]->SetState(temp2);
		else
			Pieces[i]->SetState(temp3);
	}
}
void CSquare::Draw()
{
	CGame* game = CGame::GetInstance();
	game->Draw(round(x), round(y), sprite->GetText(), sprite->GetLeft(), sprite->GetTop(), sprite->GetRight(), sprite->GetBottom(), 255);
	for (unsigned int i = 0; i < Pieces.size(); i++)
	{
		if (Pieces[i] == NULL)
			return;		
		Pieces[i]->Render();
	}

}
CSquare::~CSquare()
{

}