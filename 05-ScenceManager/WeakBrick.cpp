#pragma once
#include "WeakBrick.h"
CWeakBrick::CWeakBrick() :CGameObject()
{
	vx = 0;
	vy = 0;
	SetState(WEAKBRICK_STATE_NORMAL);
}
void CWeakBrick::Render()
{
	if (state == WEAKBRICK_STATE_DISAPPEAR)
		return;
	animation_set->at(0)->Render(round(x), round(y));		//normal bounty brick animation
	//RenderBoundingBox();
}
void CWeakBrick::ActivateFragment()
{
	if(Coin!=NULL)
		Coin->isFinished = true;
	for (UINT i = 0; i < fragments.size(); i++)
	{
		fragments[i]->isUsed = true;
		fragments[i]->isFinished = false;
		float fragment_vx = 0;
		float fragment_vy = 0;
		if (i == 0)
		{
			fragment_vx = -FRAGMENT_SPEED_VX;
			fragment_vy = -FRAGMENT_SPEED_VY;
		}
		else if (i == 1)
		{
			fragment_vx = FRAGMENT_SPEED_VX;
			fragment_vy = -FRAGMENT_SPEED_VY;
		}
		else if (i == 2)
		{
			fragment_vx = -FRAGMENT_SPEED_VX;
			fragment_vy = -FRAGMENT_SPEED_VY_2;
		}
		else
		{
			fragment_vx = FRAGMENT_SPEED_VX;
			fragment_vy = -FRAGMENT_SPEED_VY_2;
		}

		fragments[i]->SetSpeed(fragment_vx, fragment_vy);
	}
}
void CWeakBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (AABBCheck(Mario) == true)
	//	ActivateFragment();
	if (state == WEAKBRICK_STATE_DISAPPEAR)
		return;
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		return;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CKoopas*>(e->obj))
			{
				if (nx != 0)
				{
					CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
					float koopas_vx, koopas_vy;
					if (state == BOUNTYBRICK_STATE_NORMAL)
					{
						if (koopas->GetState() == KOOPAS_STATE_DEFENSE_DYNAMIC)
						{
							if (state == WEAKBRICK_STATE_NORMAL)
							{
								
								koopas->GetSpeed(koopas_vx, koopas_vy);
								koopas->SetSpeed(-koopas_vx, koopas_vy);
								ActivateFragment();
								state = WEAKBRICK_STATE_DISAPPEAR;
								MoveWeakBrickToHorizon();							
							}
						}
						else
						{
							koopas->GetSpeed(koopas_vx, koopas_vy);
							koopas->SetSpeed(-koopas_vx, koopas_vy);
						}
					}
				}
			}

		}
	}
}
void CWeakBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WEAKBRICK_BBOX_WIDTH;
	b = y + WEAKBRICK_BBOX_HEIGHT;
}
void CWeakBrick::SetState(int state)
{
	CGameObject::SetState(state);

}
CWeakBrick::~CWeakBrick()
{

}