#include"FireFlower.h"
#include"Utils.h"
CFireFlower::CFireFlower()
{
	SetState(FIREFLOWER_STATE_LEFT_LOWER);
	vy = -FIREFLOWER_FLY_SPEED;
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
	CGameObject::Update(dt, coObjects);	
	y += dy;	
	if (y <= FIREFLOWR_APPEAR_Y)
		isAppear = true;
	else
		isAppear = false;
	if (y < FIREFLOWER_UPPER_Y )
	{
		if (GetTickCount64() - Shoot_Start > FIREFLOWER_TIMEWAIT_FORSHOOT)
		{
			isShooting = false;
			Shoot_Start = 0;
			Shoot();
			vy = FIREFLOWER_FLY_SPEED;
		}
		else
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
			FireBallFlower->SetPosition(x + 18, y + 5);
			FireBallFlower->SetSpeed(FIREBALLFLOWER_SPEED_X, FIREBALLFLOWER_SPEED_Y);
			//fireballs->SetDirectionnx(1);
		}
		else if (state == FIREFLOWER_STATE_RIGHT_UPPER)
		{
			FireBallFlower->SetPosition(x + 18, y + 5);
			FireBallFlower->SetSpeed(FIREBALLFLOWER_SPEED_X, -FIREBALLFLOWER_SPEED_Y);
		}
		else if(state==FIREFLOWER_STATE_LEFT_LOWER)
		{
			FireBallFlower->SetPosition(x - 2, y + 5);
			FireBallFlower->SetSpeed(-FIREBALLFLOWER_SPEED_X, FIREBALLFLOWER_SPEED_Y);
			//fireballs->SetDirectionnx(-1);
		}
		else if (state == FIREFLOWER_STATE_LEFT_UPPER)
		{
			FireBallFlower->SetPosition(x - 2, y + 5);
			FireBallFlower->SetSpeed(-FIREBALLFLOWER_SPEED_X, -FIREBALLFLOWER_SPEED_Y);
		}

		FireBallFlower->isUsed = true;
		FireBallFlower->isFinished = false;
	}
}
void CFireFlower::Render()
{
	int ani = -1;
	if (state == FIREFLOWER_STATE_RIGHT_UPPER)
		ani = FIREFLOWER_ANI_RIGHT_UPPER;
	else if (state == FIREFLOWER_STATE_LEFT_UPPER)
		ani = FIREFLOWER_ANI_LEFT_UPPER;
	else if (state == FIREFLOWER_STATE_RIGHT_LOWER)
		ani = FIREFLOWER_ANI_RIGHT_LOWER;
	else
		ani = FIREFLOWER_ANI_LEFT_LOWER;
	animation_set->at(ani)->Render(x, y);	
	RenderBoundingBox();
}
void CFireFlower::SetState(int state)
{
	CGameObject::SetState(state);
	
}