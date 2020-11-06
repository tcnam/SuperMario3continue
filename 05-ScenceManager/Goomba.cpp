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
	CalcPotentialCollisions(&BrickObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_tyduong,min_tyam, nx=0 , ny=0;
		float rdx = 0;
		float rdy = 0;
		
		min_tx = 1.0f;
		min_tyduong = min_tyam=1.0f;
		int min_ix = -1;
		int min_iyduong = -1;
		int min_iyam = -1;
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
			if (c->ny > 0)
			{
				if (c->t < min_tyduong)
				{
					min_tyduong = c->t;
					ny = c->ny;
					min_iyduong = i;
					rdy = c->dy;
				}
			}				
			else
			{
				if (c->t < min_tyam )
				{
					min_tyam = c->t;
					ny = c->ny;
					min_iyam = i;
					rdy = c->dy;
				}
			}
			if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
			if (min_iyduong >= 0) coEventsResult.push_back(coEvents[min_iyduong]);
			if (min_iyam >= 0) coEventsResult.push_back(coEvents[min_iyam]);
		}
		x += min_tx * dx + nx * 0.4f;
		y += min_tyam * dy + ny * 0.4f;

		if (nx != 0) vx = -vx;
		if (ny != 0) vy = 0;
		/*for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (e->nx != 0)
				{
					goomba->vx = -goomba->vx;
					vx = -vx;
				}
			}
		}*/
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 


	/*if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/
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
			y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			vx = 0;
			vy = 0;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
	}
}
