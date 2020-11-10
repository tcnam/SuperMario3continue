#include "Goomba.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;
	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else 	
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += GOOMBA_GRAVITY * dt;	
	vector<LPGAMEOBJECT> BrickObjects;
	BrickObjects.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == OBJECT_TYPE_BRICK/*|| coObjects->at(i)->GetType() == OBJECT_TYPE_GOOMBA*/)
			BrickObjects.push_back(coObjects->at(i));
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if(state!=GOOMBA_STATE_DIE)
		CalcPotentialCollisions(&BrickObjects, coEvents);	
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx=0 , ny=0;
		float rdx = 0;
		float rdy = 0;
		
		min_tx = 1.0f;
		min_ty=1.0f;
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

		if (nx != 0) vx=-vx;
		if (ny < 0) vy = 0;
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animation_set->at(ani)->Render(x,y);

	RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE ;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING:
			vx = -GOOMBA_WALKING_SPEED;

	}
}
