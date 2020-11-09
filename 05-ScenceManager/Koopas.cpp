#include "Koopas.h"

CKoopas::CKoopas()
{
	right();
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state != KOOPAS_STATE_WALKING)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy += KOOPAS_GRAVITY * dt;
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
		//isOnGround = false;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		/*LPCOLLISIONEVENT ey = coEvents[min_ty];
		if (dynamic_cast<CBrick*>(ey->obj))
		{

		}
		LPCOLLISIONEVENT ex = coEvents[min_tx];*/

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0) vx = -vx;
		if (ny < 0)
		{
			vy = 0;
		}
	}
}

void CKoopas::Render()
{
	int ani = -1;
	/*if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}*/
		
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx > 0)
			ani = KOOPAS_ANI_WALKING_RIGHT;
		else
			ani = KOOPAS_ANI_WALKING_LEFT;
	}
	else if (state == KOOPAS_ANI_DEFENSE_DYNAMIC && vx!=0)ani = KOOPAS_ANI_DEFENSE_DYNAMIC;
	else
		ani = KOOPAS_ANI_DEFENSE_STATIC;
	
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}
void CKoopas::stop()
{
	if (abs(vx) == KOOPAS_DYNAMIC_SPEED)
	{
		vx = 0;
		vy = 0;
	}
	else
	{
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
	}
}
void CKoopas::left()
{
	nx = -1;
}
void CKoopas::right()
{
	nx = 1;
}
void CKoopas::walk()
{
	vx = nx*KOOPAS_WALKING_SPEED;
}
void CKoopas::dynamic()
{
	vx = nx * KOOPAS_DYNAMIC_SPEED;
}
void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	//case KOOPAS_STATE_DIE:
		//y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE -1;
		//vx = 0;
		//vy = 0;
		//break;
	case KOOPAS_STATE_WALKING:
		walk();
		break;
	case KOOPAS_STATE_DEFENSE_STATIC:
		stop();
		break;
	case KOOPAS_STATE_DEFENSE_DYNAMIC:
		dynamic();
		break;
	}

}