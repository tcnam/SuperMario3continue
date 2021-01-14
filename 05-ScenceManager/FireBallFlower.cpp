#include"FireBallFLower.h"

CFireBallFlower::CFireBallFlower()
{
	vx = 0;
	vy = 0;
	isUsed = false;
	isFinished = false;
	Mario = NULL;
}

void CFireBallFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (Mario == NULL)
	//	return;
	if (isFinished == true)
		return;
	if (isUsed == false)
		return;
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
		isFinished = true;
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
				Mario->SetLevel(Mario->GetLevel() - 1);
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
			isFinished = true;
			
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
					Mario->SetLevel(Mario->GetLevel() - 1);
				}
			}
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}
void CFireBallFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIREBALLFLOWER_BBOX_WIDTH;
	bottom = y + FIREBALLFLOWER_BBOX_HEIGHT;
}
void CFireBallFlower::Render()
{
	if (isFinished == true)
		return;
	animation_set->at(1)->Render(round(x), round(y));
}
CFireBallFlower::~CFireBallFlower()
{

}