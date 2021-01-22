#include"FireFlower.h"
#include"Utils.h"
CFireFlower::CFireFlower()
{
	SetState(FIREFLOWER_STATE_LEFT_LOWER);
	vy = -FIREFLOWER_FLY_SPEED;
	isFinish = false;
	start_x = 0;
	start_y = 0;
	Finish_Start = 0;
}
void CFireFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FIREFLOWER_BBOX_WIDTH;
	bottom = y + FIREFLOWER_BBOX_HEIGHT;
}
void CFireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario == NULL)
		return;
	if (Mario->GetState() == MARIO_STATE_DIE)
		return;
	if (Mario->isTransform == true)
		return;
	if (Finish_Start != 0)
	{
		if (GetTickCount64() - Finish_Start > FIREFLOWER_TIMEWAIT_FORSHOOT)
		{
			Finish_Start = 0;
			effect->SetPosition(EFFECT_LOCATION_X_TO_HIDE, EFFECT_LOCATION_Y_TO_HIDE);
		}
	}	
	if (isFinish == true)
	{
		//effect->SetPosition(EFFECT_LOCATION_X_TO_HIDE,EFFECT_LOCATION_Y_TO_HIDE);
		return;
	}
		
	if (Shoot_Start != 0)
	{
		if (GetTickCount64() - Shoot_Start > FIREFLOWER_TIMEWAIT_FORSHOOT)
		{
			isShooting = false;
			Shoot_Start = 0;
			Shoot();
			vy = FIREFLOWER_FLY_SPEED;
		}
	}
	
	float Mario_x, Mario_y;
	Mario->GetPosition(Mario_x, Mario_y);
	if (isAppear == true)
	{
		if (Mario_x> x + 16)
		{
			if (Mario_y < FIREFLOWER_UPPER_Y)
				state=FIREFLOWER_STATE_RIGHT_UPPER;
			else
				state=FIREFLOWER_STATE_RIGHT_LOWER;
		}
		else
		{
			if (Mario_y < FIREFLOWER_UPPER_Y)
				state=FIREFLOWER_STATE_LEFT_UPPER;
			else
				state=FIREFLOWER_STATE_LEFT_LOWER;
		}
	}
	else
	{

		if (Mario_x > x + BONUS_DISTANCE_NOT_SHOWUP)
		{
			SetSpeed(0, -FIREFLOWER_FLY_SPEED);
			if (y < FIREFLOWER_UPPER_Y)
				state=FIREFLOWER_STATE_RIGHT_UPPER;
			else
				state=FIREFLOWER_STATE_RIGHT_LOWER;
		}
		else if (Mario_x < x - BONUS_DISTANCE_NOT_SHOWUP&&Mario->GetLevel()!=MARIO_LEVEL_TAIL)
		{
			SetSpeed(0, -FIREFLOWER_FLY_SPEED);
			if (y < FIREFLOWER_UPPER_Y)
				state=FIREFLOWER_STATE_LEFT_UPPER;
			else
				state=FIREFLOWER_STATE_LEFT_LOWER;
		}
		else if (Mario_x < x - BONUS_DISTANCE_NOT_SHOWUP2 && Mario->GetLevel() == MARIO_LEVEL_TAIL)
		{
			SetSpeed(0, -FIREFLOWER_FLY_SPEED);
			if (y < FIREFLOWER_UPPER_Y)
				state = FIREFLOWER_STATE_LEFT_UPPER;
			else
				state = FIREFLOWER_STATE_LEFT_LOWER;
		}
		else
		{
			SetSpeed(0, 0);
		}
	}
	CGameObject::Update(dt, coObjects);	
	if (AABBCheck(Mario) == true)
	{
		if (Mario->untouchable == false&&Mario->isHoldingKoopas==false)
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
			if (Mario->GetEffect() != NULL)
			{
				Mario->GetEffect()->SetState(EFFECT_CLOUND);
				Mario->GetEffect()->SetPosition(Mario_x, Mario_y);
				Mario->StartTransForm();
			}
		}
		else if (Mario->isHoldingKoopas == true)
		{
			if (effect != NULL)
			{
				effect->SetPosition(x, y);
				effect->SetState(EFFECT_CLOUND);
			}
			SetPosition(start_x, start_y);
			StartFinish();
		}
	}
	y += dy;	
	if (y <= FIREFLOWR_APPEAR_Y)
		isAppear = true;
	else
	{
		isAppear = false;
	}
		
	if (y < FIREFLOWER_UPPER_Y )
	{
		if(Shoot_Start==0)
			StartShoot();			
	}
	else if (y >= FIREFLOWER_LOWER_Y)
	{
		vy = -FIREFLOWER_FLY_SPEED;
	}

		
}
void CFireFlower::Shoot()
{
	if (FireBallFlower == NULL)
	{
		DebugOut(L"There arn't no fire ball");
		return;
	}
	else
	{
		if (state==FIREFLOWER_STATE_RIGHT_LOWER)
		{
			FireBallFlower->SetPosition(x + RELATIVE_POSITION_X_RIGHT_FIREBALL, y + RELATIVE_POSITION_Y_FIREBALL);
			FireBallFlower->SetSpeed(FIREBALLFLOWER_SPEED_X, FIREBALLFLOWER_SPEED_Y);
			//fireballs->SetDirectionnx(1);
		}
		else if (state == FIREFLOWER_STATE_RIGHT_UPPER)
		{
			FireBallFlower->SetPosition(x + RELATIVE_POSITION_X_RIGHT_FIREBALL, y + RELATIVE_POSITION_Y_FIREBALL);
			FireBallFlower->SetSpeed(FIREBALLFLOWER_SPEED_X, -FIREBALLFLOWER_SPEED_Y);
		}
		else if(state==FIREFLOWER_STATE_LEFT_LOWER)
		{
			FireBallFlower->SetPosition(x - RELATIVE_POSITION_X_LEFT_FIREBALL, y + RELATIVE_POSITION_Y_FIREBALL);
			FireBallFlower->SetSpeed(-FIREBALLFLOWER_SPEED_X, FIREBALLFLOWER_SPEED_Y);
			//fireballs->SetDirectionnx(-1);
		}
		else if (state == FIREFLOWER_STATE_LEFT_UPPER)
		{
			FireBallFlower->SetPosition(x - RELATIVE_POSITION_X_LEFT_FIREBALL, y + RELATIVE_POSITION_Y_FIREBALL);
			FireBallFlower->SetSpeed(-FIREBALLFLOWER_SPEED_X, -FIREBALLFLOWER_SPEED_Y);
		}

		FireBallFlower->isUsed = true;
		FireBallFlower->isFinished = false;
	}
}
void CFireFlower::Render()
{
	if (isFinish == true)
		return;
	int ani = -1;
	if (state == FIREFLOWER_STATE_RIGHT_UPPER)
		ani = FIREFLOWER_ANI_RIGHT_UPPER;
	else if (state == FIREFLOWER_STATE_LEFT_UPPER)
		ani = FIREFLOWER_ANI_LEFT_UPPER;
	else if (state == FIREFLOWER_STATE_RIGHT_LOWER)
		ani = FIREFLOWER_ANI_RIGHT_LOWER;
	else
		ani = FIREFLOWER_ANI_LEFT_LOWER;
	animation_set->at(ani)->Render(round(x), round(y));	
	//RenderBoundingBox();
}
void CFireFlower::SetState(int state)
{
	CGameObject::SetState(state);
}
CFireFlower::~CFireFlower()
{

}