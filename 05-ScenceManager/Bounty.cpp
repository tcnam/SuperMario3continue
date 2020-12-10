#include"Bounty.h"
CBounty::CBounty()
{
	isUsed = false;
	isLeaf = false;
	isActive = false;
}
void CBounty::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOUNTY_BBOX_WIDTH;
	bottom = y + BOUNTY_BBOX_HEIGHT;
}

void CBounty::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (isUsed == false)
		return;
	if ((DWORD)GetTickCount64()-Bounty_Start>BOUNTY_ACTIVE_TIME)
	{
		
		isActive = false;
		Bounty_Start = 0;
	}
	else
	{
		if (state == BOUNTY_STATE_POWERUP || state == BOUNTY_STATE_LIFEUP)
		{
			vy = -BOUNTY_OTHERS_VY;
		}
		else
			vy = -BOUNTY_COIN_VY;
	}
	vy += BOUNTY_GRAVITY * dt;
	vector<LPGAMEOBJECT> BrickObjects;
	BrickObjects.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == OBJECT_TYPE_BRICK || coObjects->at(i)->GetType() == OBJECT_TYPE_HIDDENOBJECT || coObjects->at(i)->GetType() == OBJECT_TYPE_BOUNTYBRICK)
			BrickObjects.push_back(coObjects->at(i));
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
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

		min_tx = 1.0f;
		min_ty = 1.0f;
		int min_ix = -1;
		int min_iy = -1;
		coEventsResult.clear();

		for (UINT i = 0; i < coEvents.size(); i++)
		{
			LPCOLLISIONEVENT c = coEvents[i];
			if (c->nx * vx > 0)
				return;
			else
			{
				if (c->t < min_tx)
				{
					min_tx = c->t;
					nx = c->nx;
					min_ix = i;
					rdx = c->dx;
				}
			}
			if (c->ny < 0)
			{
				if (c->t < min_ty)
				{
					min_ty = c->t;
					ny = c->ny;
					min_iy = i;
					rdy = c->dy;
				}
			}
			if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
			if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
		}
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = -vx;
		if (ny < 0)
		{
			if (state == BOUNTY_STATE_POWERUP || state == BOUNTY_STATE_LIFEUP)
			{
				vx = -BOUNTY_OTHERS_VX;
			}
			vy = 0;

		}
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	
}

void CBounty::Render()
{
	//if (isUsed == false)
	//	return;
	int ani = -1;
	if (state == BOUNTY_STATE_POWERUP)
	{
		if (isLeaf == true)
			ani = BOUNTY_LEAF_ANI;
		else
			ani = BOUNTY_RED_MUSHROOM_ANI;
	}
	else if (state == BOUNTY_STATE_LIFEUP)
	{
		ani = BOUNTY_GREEN_MUSHROOM_ANI;
	}
	else
		ani = BOUNTY_COIN_ANI;
	animation_set->at(ani)->Render(x, y);
}

void CBounty::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOUNTY_STATE_COIN:
		vy = 0;
		vx = 0;
		break;
	case BOUNTY_STATE_POWERUP:
		vy = 0;
		vx = 0;
		break;
	case BOUNTY_STATE_LIFEUP:
		vx = 0;
		vy = 0;
		break;

	}
}