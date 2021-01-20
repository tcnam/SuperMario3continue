#include "BoomerangBro.h"
CBoomerangBro::CBoomerangBro()
{
	start_x = 0.0f;
	start_y = 0.0f;
	SetState(BOOMERANGBRO_STATE_HOLD);
	vx = BOOMERANGBRO_VX;
	Mario = NULL;
	Boomerang = NULL;
}
CBoomerangBro::~CBoomerangBro()
{

}
void CBoomerangBro::ThrowBoomerang()
{
	if (Boomerang == NULL)
		return;
	if (Boomerang->isUsed == true)
		return;
	if(Boomerang->isUsed==false)
		Boomerang->isUsed = true;	
	Boomerang->SetSpeed(0.05f, -0.025f);
}
void CBoomerangBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario == NULL)
		return;
	if (Mario->GetState() == MARIO_STATE_DIE)
		return;
	if (Mario->isTransform == true)
		return;
	float Mario_x, Mario_y;
	Mario->GetPosition(Mario_x, Mario_y);
	if (x < start_x - DISTANCE_DX_TO_CHANGE_SPEED)
	{
		vx = BOOMERANGBRO_VX;
		ThrowBoomerang();
	}
	else if (x >= start_x + DISTANCE_DX_TO_CHANGE_SPEED)
	{
		vx = -BOOMERANGBRO_VX;
	}
	CGameObject::Update(dt, coObjects);
	vy += BOOMERANG_GRAVITY * dt;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (state == BOOMERANGBRO_STATE_HOLD || state == BOOMERANGBRO_STATE_NO_HOLD)
		CalcPotentialCollisions(coObjects, coEvents);
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
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		/*if (nx != 0) vx=-vx;*/
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{
				SetPosition(x, y - 1);
				if (nx != 0 && ny == 0)
				{
					if (Mario->untouchable == false)
					{
						if (Mario->GetLevel() == MARIO_LEVEL_SMALL)
							Mario->SetState(MARIO_STATE_DIE);
						else
						{
							Mario->StartUntouchable();
							if (Mario->GetLevel() == MARIO_LEVEL_FIRE || Mario->GetLevel() == MARIO_LEVEL_TAIL)
								Mario->SetLevel(MARIO_LEVEL_BIG);
							else
								Mario->SetLevel(MARIO_LEVEL_SMALL);
						}
					}
				}
				if (ny > 0 && nx == 0)
				{
					float Mario_vx, Mario_vy;
					Mario->GetSpeed(Mario_vx, Mario_vy);
					Mario->SetSpeed(Mario_vx, -MARIO_JUMP_DEFLECT_SPEED);
					if (state != BOOMERANGBRO_STATE_DIE)
					{
						state = BOOMERANGBRO_STATE_DIE;
						Boomerang->isFinish = true;
						CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
						vx = 0;
						vy = 0;
					}
				}
				if (ny < 0 && nx == 0)
				{
					Mario->SetPosition(Mario_x, Mario_y - 1);
					if (Mario->untouchable == false)
					{
						if (Mario->GetLevel() == MARIO_LEVEL_SMALL)
							Mario->SetState(MARIO_STATE_DIE);
						else
						{
							Mario->StartUntouchable();
							if (Mario->GetLevel() == MARIO_LEVEL_FIRE || Mario->GetLevel() == MARIO_LEVEL_TAIL)
								Mario->SetLevel(MARIO_LEVEL_BIG);
							else
								Mario->SetLevel(MARIO_LEVEL_SMALL);
						}
					}
				}
			}
			
		}
		for (UINT i = 0; i < coEvents.size(); i++)
			delete coEvents[i];
		if (Boomerang != NULL)
		{
			if (Boomerang->isUsed == false)
			{
				Boomerang->SetInitPositon(x-8, y-5);
				Boomerang->SetPosition(x-8, y-5);
			}
			else
			{
				Boomerang->SetInitPositon(x - 8, y - 5);
			}
		}
	}
}
void CBoomerangBro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANGBRO_BBOX_WIDTH;
	bottom = y + BOOMERANGBRO_BBOX_HEIGHT;
}
void CBoomerangBro::Render()
{
	if (state == BOOMERANGBRO_STATE_DIE)
		animation_set->at(2)->Render(round(x), round(y));
	else if (state == BOOMERANGBRO_STATE_NO_HOLD)
		animation_set->at(1)->Render(round(x), round(y));
	else
		animation_set->at(0)->Render(round(x), round(y));
}