#include"FireFlower.h"
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
	bottom = x + FIREFLOWER_BBOX_HEIGHT;
}
void CFireFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);	
	y += dy;
	
	if (y <= FIREFLOWER_UPPER_Y || y >= FIREFLOWER_LOWER_Y)
	{
		if (GetTickCount64() - Shoot_Start > FIREFLOWER_TIMEWAIT_FORSHOOT)
		{
			isShooting = false;
			Shoot_Start = 0;
		}
		else
			StartShoot();
		vy = -vy;
			
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