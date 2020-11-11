#include "Koopas.h"

CKoopas::CKoopas()
{
	right();
	SetState(KOOPAS_STATE_DEFENSE_STATIC);
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
	
	if (state == KOOPAS_STATE_ISHOLD)
	{
		vx = 0;
		vy = 0;
	}
	else
	{
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

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
			//if (nx != 0) vx = -vx;
			if (ny < 0)
			{
				vy = 0;
			}
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (nx != 0)
				{
					if (dynamic_cast<CBrick*>(e->obj))
					{
						vx = -vx;
					}
					else if (dynamic_cast<CBountyBrick*>(e->obj))
					{
						vx = -vx;
					}
					else if (dynamic_cast<CHiddenObject*>(e->obj))
					{
						if (state != KOOPAS_STATE_WALKING)
						{
							x += dx;
							y += dy;
						}
						else
							vx = -vx;
					}
					else if (dynamic_cast<CGoomba*>(e->obj))
					{
						if (state != KOOPAS_STATE_WALKING)
						{
							x += dx;
							//y += dy;
							CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
							if (goomba->GetState() != GOOMBA_STATE_DIE)
								goomba->SetState(GOOMBA_STATE_DIE);
						}
						else
							vx = -vx;

					}
				}
			}
		}
	
	}
	
}

void CKoopas::Render()
{
	int ani = -1;
	/*if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}*/
		
	if (vx == 0)
		ani = KOOPAS_ANI_DEFENSE_STATIC;
	else if(vx>0)
	{
		if (state == KOOPAS_STATE_WALKING)
			ani = KOOPAS_ANI_WALKING_RIGHT;
		else
			ani = KOOPAS_ANI_DEFENSE_DYNAMIC;
	}
	else
	{
		if (state == KOOPAS_STATE_WALKING)
			ani = KOOPAS_ANI_WALKING_LEFT;
		else
			ani = KOOPAS_ANI_DEFENSE_DYNAMIC;
	}
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