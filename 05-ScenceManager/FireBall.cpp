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
	nx = -1;
	isUsed = false;
	isFinished = true;;
}

void CFireBall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFinished == true)
		return;
	CGameObject::Update(dt);
	if(isUsed==true)
		vy += FIREBALL_GRAVITY*dt;
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
			vy =-FIREBALL_DEFLECT_SPEED;
		}
		if (nx != 0)
		{
			isFinished = true;
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
		}
	}
	//DebugOut(L"--> %s\n", ToWSTR(nx.ToString());
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
		animation_set->at(0)->Render(round(x), round(y));
	else
		animation_set->at(1)->Render(round(x), round(y));
}
CFireBall::~CFireBall()
{

}