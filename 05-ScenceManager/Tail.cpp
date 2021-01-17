#include"Tail.h"
#include"BountyBrick.h"
#include"SpecialBrick.h"
#include"Goomba.h"
#include"Koopas.h"
#include"FireFlower.h"
#include"Piranha.h"

CTail::CTail()
{
	vx = 0;
	vy = 0;
	isUsed = false;
	start_x = 0;
	start_y = 0;
}

void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUsed == false)
		return;
	CGameObject::Update(dt);
	if (abs(x - start_x) >= TAIL_DX_MOVE)
	{
		vy = 0;
		vx = 0;
		isUsed = false;
		start_x = x;
		start_y = y;
	}
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
				isUsed = false;
				CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
				if (goomba->GetLevel() == GOOMBA_LEVEL_FLY)
				{
					goomba->Setlevel(GOOMBA_LEVEL_NORMAL);
					goomba->SetState(GOOMBA_STATE_KICKED_OUT);
					goomba->SetSpeed(0, -0.2f);
					CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
				}
				else
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_KICKED_OUT);
						goomba->SetSpeed(0, -0.2f);
					}
					CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
				}
				
			} // if Goomba
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				isUsed = false;
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (koopas->GetLevel() == KOOPAS_LEVEL_FLY)
					koopas->Setlevel(KOOPAS_LEVEL_NORMAL);
				koopas->SetState(KOOPAS_STATE_KICKOUT_2);
				koopas->SetSpeed(0, -0.24f);
				koopas->StartWait();
				CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
			}
			else if (dynamic_cast<CSpecialBrick*>(e->obj))
			{
				isUsed = false;
				CSpecialBrick* spB = dynamic_cast<CSpecialBrick*>(e->obj);
				if (spB->GetState() != SPECIALBRICK_STATE_EMPTY)
				{
					spB->SetState(SPECIALBRICK_STATE_EMPTY);
					spB->ActivateBountyButton();
				}					
			}
			else if (dynamic_cast<CBountyBrick*>(e->obj))
			{
				isUsed = false;
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
				isUsed = false;
				CWeakBrick* weakbrick = dynamic_cast<CWeakBrick*>(e->obj);
				if (weakbrick->GetState() == WEAKBRICK_STATE_NORMAL)
				{
					weakbrick->SetState(WEAKBRICK_STATE_DISAPPEAR);
					weakbrick->ActivateFragment();
					weakbrick->MoveWeakBrickToHorizon();

				}
			}
			else if (dynamic_cast<CFireFlower*>(e->obj))
			{
				isUsed = false;
				CFireFlower* fireflower = dynamic_cast<CFireFlower*>(e->obj);
				float flower_x, flower_y;
				fireflower->GetPosition(flower_x, flower_y);
				if (fireflower->isFinish != true)
				{
					if (fireflower->GetEffect() != NULL)
					{
						fireflower->GetEffect()->SetPosition(flower_x, flower_y);
						fireflower->GetEffect()->SetState(EFFECT_CLOUND);
					}
					fireflower->SetPosition(fireflower->GetStartx(), fireflower->GetStarty());
					fireflower->StartFinish();
				}	
				CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
			}
			else if (dynamic_cast<CPiranha*>(e->obj))
			{
				CPiranha* piranha = dynamic_cast<CPiranha*>(e->obj);
				if (piranha->isFinish != true)
				{
					piranha->SetPosition(piranha->GetStartx(), piranha->GetStarty());
					piranha->StartFinish();
				}
				CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
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
	//RenderBoundingBox();
}
CTail::~CTail()
{

}