#include "BountyBrick.h"
CBountyBrick::CBountyBrick() :CGameObject()
{
	vx = 0;
	vy = 0;
	SetState(BOUNTYBRICK_STATE_NORMAL);
}
void CBountyBrick::Render()
{
	if (state == BOUNTYBRICK_STATE_EMPTY)
		animation_set->at(1)->Render(x, y);		//empty bounty brick animation
	else if(state==BOUNTYBRICK_STATE_NORMAL)
		animation_set->at(0)->Render(x, y);		//normal bounty brick animation
	//RenderBoundingBox();
}
void CBountyBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	y += dy;
	if (y < start_y - BOUNTYBRICK_MOVING_DISTANCE_TOBACK)
	{
		y = start_y - BOUNTYBRICK_MOVING_DISTANCE_TOBACK;
		vy = BOUNTYBRICK_SPEED_Y;
	}
	else if (y > start_y)
	{
		y = start_y;
		vy = 0;
	}
	vector<LPGAMEOBJECT> RealCoObjects;
	RealCoObjects.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == OBJECT_TYPE_MARIO|| coObjects->at(i)->GetType() == OBJECT_TYPE_KOOPAS)
			RealCoObjects.push_back(coObjects->at(i));
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (state != BOUNTYBRICK_STATE_EMPTY)
		CalcPotentialCollisions(&RealCoObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = -vx;
		if (ny < 0) vy = 0;*/
		for (UINT i = 0; i < coEvents.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{
				float mario_vx, mario_vy;
				float mario_x, mario_y;

				Mario->GetPosition(mario_x, mario_y);
				Mario->GetSpeed(mario_vx, mario_vy);


				if (ny < 0)
				{

					Mario->SetPosition(mario_x, start_y + BOUNTY_BBOX_HEIGHT + 0.001);
					Mario->SetSpeed(mario_vx, 0);
					if (state == BOUNTYBRICK_STATE_NORMAL)
					{
						state = BOUNTYBRICK_STATE_EMPTY;
						vy = -BOUNTYBRICK_SPEED_Y;
						if (Mario->GetLevel() > MARIO_LEVEL_SMALL)
							Bounty->isLeaf = true;
						else
							Bounty->isLeaf = false;
						if (vx > 0)
							Bounty->isRightDirection = true;
						else
							Bounty->isRightDirection = false;
						Bounty->isUsed = true;
						ActivateBounty();
					}
				}
			}
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				if (nx != 0)
				{
					if (state == BOUNTYBRICK_STATE_NORMAL)
					{
						state = BOUNTYBRICK_STATE_EMPTY;
						if (Mario->GetLevel() > MARIO_LEVEL_SMALL)
							Bounty->isLeaf = true;
						else
							Bounty->isLeaf = false;
						if (vx > 0)
							Bounty->isRightDirection = true;
						else
							Bounty->isRightDirection = false;
						Bounty->isUsed = true;
						ActivateBounty();
					}
				}
			}
		}		
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];	
}

void CBountyBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
void CBountyBrick::SetState(int state)
{
	CGameObject::SetState(state);
}