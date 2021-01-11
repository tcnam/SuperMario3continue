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
	if (isUsed == false)
		return;
	CGameObject::Update(dt);
	y += dy;
	x += dx;

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
	animation_set->at(1)->Render(round(x), round(y));
}