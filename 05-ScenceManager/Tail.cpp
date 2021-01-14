#include"Tail.h"
#include"BountyBrick.h"
#include"SpecialBrick.h"
#include"Goomba.h"
#include"Koopas.h"

CTail::CTail()
{
	vx = 0;
	vy = 0;
	isUsed = false;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);
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
		if (nx != 0)
		{
			vx = 0;
		}
		if (ny != 0)
		{
			vy = 0;
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
					goomba->SetSpeed(0, -0.4f);
				}
			} // if Goomba
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (koopas->GetState() != KOOPAS_STATE_DEFENSE_STATIC)
				{
					koopas->SetState(KOOPAS_STATE_DEFENSE_STATIC);
					koopas->SetSpeed(0, -0.4f);
				}
			}
			else if (dynamic_cast<CSpecialBrick*>(e->obj))
			{
				CSpecialBrick* spB = dynamic_cast<CSpecialBrick*>(e->obj);
				if (spB->GetState() != SPECIALBRICK_STATE_EMPTY)
				{
					spB->SetState(SPECIALBRICK_STATE_EMPTY);
					spB->ActivateBountyButton();
				}					
			}
			else if (dynamic_cast<CBountyBrick*>(e->obj))
			{
				CBountyBrick* bountybrick = dynamic_cast<CBountyBrick*>(e->obj);
				if (bountybrick->GetState()!= BOUNTYBRICK_STATE_EMPTY)
				{
					bountybrick->SetState(BOUNTYBRICK_STATE_EMPTY);
					bountybrick->GetBounty()->isLeaf = true;
					bountybrick->ActivateBounty();					
				}
			}
			else if (dynamic_cast<CWeakBrick*>(e->obj))
			{
				CWeakBrick* weakbrick = dynamic_cast<CWeakBrick*>(e->obj);
				if (weakbrick->GetState() == WEAKBRICK_STATE_NORMAL)
				{
					weakbrick->SetState(WEAKBRICK_STATE_DISAPPEAR);
					weakbrick->ActivateFragment();
					weakbrick->MoveWeakBrickToHorizon();

				}
			}
		}
	}
	//DebugOut(L"--> %s\n", ToWSTR(nx.ToString());
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CTail::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIREBALL_BBOX_WIDTH;
	bottom = y + FIREBALL_BBOX_HEIGHT;
}
void CTail::Render()
{
	RenderBoundingBox();
}
CTail::~CTail()
{

}