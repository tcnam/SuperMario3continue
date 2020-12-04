#include"FireBallFLower.h"

CFireBallFlower::CFireBallFlower()
{
	vx = 0;
	vy = 0;
	isUsed = false;
	isFinished = false;
}

void CFireBallFlower::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFinished == true)
		return;
	CGameObject::Update(dt);

}
void CFireBallFlower::Attack(float vx, float vy, int nx)
{
	
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
	if (nx > 0)
		animation_set->at(0)->Render(x, y);
	else
		animation_set->at(1)->Render(x, y);
}