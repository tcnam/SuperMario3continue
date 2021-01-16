#include "BountyButton.h"

CBountyButton::CBountyButton()
{
	isFinished = false;
	Mario = NULL;
	isInsideSpecialBrick = true;
	Bounty_start = 0;
}
void CBountyButton::ActivateWeakBrick()
{
	if (Bounty_start == 0)
	{
		Bounty_start = (DWORD)GetTickCount64();
	}
	for (UINT i = 0; i < weakbricks.size(); i++)
	{
		if (weakbricks[i]->GetState() == WEAKBRICK_STATE_NORMAL)
		{
			weakbricks[i]->GetCoin()->isInsideWeakBrick = false;
			weakbricks[i]->SetState(WEAKBRICK_STATE_DISAPPEAR);
			weakbricks[i]->MoveWeakBrickToHorizon();
		}		
	}
}
void CBountyButton::BringBackWeakBrick()
{
	for (UINT i = 0; i < weakbricks.size(); i++)
	{
		if (weakbricks[i]->GetState() == WEAKBRICK_STATE_DISAPPEAR)
		{
			if (weakbricks[i]->GetCoin()->isFinished == false)
			{
				weakbricks[i]->GetCoin()->isInsideWeakBrick = true;
				weakbricks[i]->SetState(WEAKBRICK_STATE_NORMAL);
				weakbricks[i]->SetPosition(weakbricks[i]->GetStartx(), weakbricks[i]->GetStarty());
			}
		}
	}
}
void CBountyButton::Render()
{
	if (isInsideSpecialBrick == true)
		return;
	if (isFinished == true)
		animation_set->at(1)->Render(round(x), round(y));
	else
		animation_set->at(0)->Render(round(x), round(y));
	RenderBoundingBox();
}
void CBountyButton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario == NULL)
		return;	
	if (isInsideSpecialBrick == true)
		return;
	if (Bounty_start != 0)
	{
		if (GetTickCount64() - Bounty_start > BOUNTYBUTTON_TIME_WAIT)
		{
			BringBackWeakBrick();
			Bounty_start = 0;
		}
	}
	if (isFinished == true)
		return;
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (AABBCheck(Mario) == true)
	{
		if (isFinished == false)
		{
			SetPosition(x, y + BOUNTYBUTTON_BBOX_HEIGHT - BOUNTYBUTTON_BBOX_HEIGHT_ISPRESSED);
			ActivateWeakBrick();
			isFinished = true;
		}		
	}
		
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() != 0)
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		if (nx != 0 || ny != 0)
		{
			if (isFinished == false)
			{
				ActivateWeakBrick();
				isFinished = true;
				SetPosition(x, y + BOUNTYBUTTON_BBOX_HEIGHT - BOUNTYBUTTON_BBOX_HEIGHT_ISPRESSED);
			}
			
		}			
	}
}

void CBountyButton::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (isFinished == false)
	{
		l = x;
		t = y;
		r = x + BOUNTYBUTTON_BBOX_WIDTH;
		b = y + BOUNTYBUTTON_BBOX_HEIGHT;
	}
	else
	{
		l = x;
		t = y;
		r = x + BOUNTYBUTTON_BBOX_WIDTH;
		b = y + BOUNTYBUTTON_BBOX_HEIGHT_ISPRESSED;
	}
	
}
CBountyButton::~CBountyButton()
{

}