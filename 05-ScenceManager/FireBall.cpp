#include"FireBall.h"
#include"BountyBrick.h"
#include"Brick.h"
#include"Goomba.h"
#include"HiddenObject.h"
#include"Koopas.h"

CFireBall::CFireBall()
{
	vx = 0;
	vy = 0;
	isFinished = false;
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFinished == true)
		return;
	CGameObject::Update(dt);
	vy += FIREBALL_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
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
		if (ny < 0)
		{
			vy = -FIREBALL_GRAVITY;
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba*>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

				if (goomba->GetState() != GOOMBA_STATE_DIE)
				{
					goomba->SetState(GOOMBA_STATE_DIE);
					isFinished = true;
				}				
			} // if Goomba
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (koopas->GetState() != KOOPAS_STATE_DEFENSE_STATIC)
				{
					koopas->SetState(KOOPAS_STATE_DEFENSE_STATIC);
					isFinished = true;
				}
			}
			else if (dynamic_cast<CBountyBrick*>(e->obj))
			{
				CBountyBrick* bountybrick = static_cast<CBountyBrick*>(e->obj);
				if (e->nx!= 0)
				{
					isFinished = true;
				}
			}
			else if (dynamic_cast<CHiddenObject*>(e->obj))
			{
				if (e->nx != 0)
				{
					isFinished = true;
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					isFinished = true;
				}
			}
		}
	}
	//DebugOut(L"--> %s\n", ToWSTR(nx.ToString());
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CFireBall::Attack(float x, float y, int nx)
{
	this->x = x;
	this->y = y;
	this->SetDirectionnx(nx);
	isFinished = false;
}
void CFireBall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIREBALL_BBOX_WIDTH;
	bottom = y + FIREBALL_BBOX_HEIGHT;
}
void CFireBall::Render()
{
	if (isFinished == true)
		return;
	if (nx > 0)
		animation_set->at(0)->Render(x, y);
	else
		animation_set->at(1)->Render(x, y);
}