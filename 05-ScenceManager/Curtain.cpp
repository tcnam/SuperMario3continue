#include "Curtain.h"
CCurtain::CCurtain()
{
	isFinish = false;
	vy = -CURTAIN_FLY_SPEED;
}
CCurtain::~CCurtain()
{

}
void CCurtain::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects )
{
	if (isFinish == true)
		return;
	CGameObject::Update(dt);
	y += dy;
	if (y < Y_LIMIT)
		isFinish = true;

}
void CCurtain::Render()
{
	if (isFinish == true)
		return;
	animation_set->at(0)->Render(round(x), round(y));
}
void CCurtain::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}