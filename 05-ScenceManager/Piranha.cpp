#include"Piranha.h"
#include"Utils.h"
CPiranha::CPiranha()
{
	vy = -FLOWER_FLY_SPEED;
	isFinish = false;
	start_x = 0;
	start_y = 0;
	Finish_Start = 0;
	isShooting = false;
	Shoot_Start = 0;
	Mario = NULL;
}
void CPiranha::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLOWER_BBOX_WIDTH;
	bottom = y + FLOWER_BBOX_HEIGHT;
}
void CPiranha::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario == NULL)
		return;
	if (Mario->GetState() == MARIO_STATE_DIE)
		return;
	if (Mario->isTransform == true)
		return;
	if (Finish_Start != 0)
	{
		if (GetTickCount64() - Finish_Start > 500)
		{
			Finish_Start = 0;
			//effect->SetPosition(EFFECT_LOCATION_X_TO_HIDE, EFFECT_LOCATION_Y_TO_HIDE);
		}
	}
	if (isFinish == true)
	{
		//effect->SetPosition(EFFECT_LOCATION_X_TO_HIDE,EFFECT_LOCATION_Y_TO_HIDE);
		return;
	}
	if (Shoot_Start != 0)
	{
		if (GetTickCount64() - Shoot_Start > FLOWER_TIMEWAIT)
		{
			isShooting = false;
			Shoot_Start = 0;
			if (y < FLOWER_UPPER_Y)
			{
				vy = FLOWER_FLY_SPEED;
			}
			else if (y >= FLOWER_LOWER_Y)
			{
				vy = -FLOWER_FLY_SPEED;
			}
			else vy = 0;				
		}
	}
	CGameObject::Update(dt, coObjects);
	if (AABBCheck(Mario) == true)
	{
		if (Mario->untouchable == false)
		{
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
	y += dy;
	if (y < FLOWER_UPPER_Y||y>= FLOWER_LOWER_Y)
	{
		if (Shoot_Start == 0)
			StartShoot();
	}
}
void CPiranha::Render()
{
	if (isFinish == true)
		return;
	animation_set->at(0)->Render(round(x), round(y));
	//RenderBoundingBox();
}
CPiranha::~CPiranha()
{

}