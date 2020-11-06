#include "Koopas.h"

CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += KOOPAS_GRAVITY * dt;
	vector<LPGAMEOBJECT> BrickObjects;
	BrickObjects.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
		if (coObjects->at(i)->GetType() == OBJECT_TYPE_BRICK)
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
			if (c->ny > 0)
				return;
			else
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
		if (ny != 0) vy = 0;
	}
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	/*x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}*/
}

void CKoopas::Render()
{
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = KOOPAS_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
	}

}