#include"Boomerang.h"

CBoomerang::CBoomerang()
{
	vx = 0;
	vy = 0;
	isUsed = false;
	isFinish = false;
	Mario = NULL;
	start_x = 0.0f;
	start_y = 0.0f;
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario == NULL)
		return;
	if (Mario->isTransform == true)
		return;
	if (isUsed == false)
		return;
	if (isFinish == true)
		return;
	if (x < start_x)
	{
		if(isUsed==true)
			isUsed = false;
	}		
	else if (x >= start_x + DISTANCE_DX_TOCHANGE_VX)
		vx = -vx;
	CGameObject::Update(dt, coObjects);
	y += dy;
	x += dx;
	float Mario_current_x, Mario_current_y;
	Mario->GetPosition(Mario_current_x, Mario_current_y);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (AABBCheck(Mario) == true)
	{
		if (Mario->untouchable == false)
		{
			if (Mario->GetEffect() != NULL)
			{
				Mario->GetEffect()->SetState(EFFECT_CLOUND);
				Mario->GetEffect()->SetPosition(Mario_current_x, Mario_current_y);
				Mario->StartTransForm();
			}
			SetPosition(x, y - 1);
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
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() != 0)
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		if (nx != 0 || ny != 0)
		{
			if (Mario->untouchable == false)
			{
				if (Mario->GetEffect() != NULL)
				{
					Mario->GetEffect()->SetState(EFFECT_CLOUND);
					Mario->GetEffect()->SetPosition(Mario_current_x, Mario_current_y);
					Mario->StartTransForm();
				}
				SetPosition(x, y - 1);
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
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMERANG_BBOX_WIDTH;
	bottom = y + BOOMERANG_BBOX_HEIGHT;
}
void CBoomerang::Render()
{
	if (isFinish == true)
		return;
	if(isUsed==false)
		animation_set->at(0)->Render(round(x), round(y));
	else
		animation_set->at(1)->Render(round(x), round(y));
}
CBoomerang::~CBoomerang()
{

}