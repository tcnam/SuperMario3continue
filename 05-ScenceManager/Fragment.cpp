#include "Fragment.h"

CFragment::CFragment()
{
	vx = 0;
	vy = 0;
	start_x = 0.0f;
	start_y = 0.0f;
	isUsed = false;
	isFinished = false;
}
void CFragment::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isUsed == false)
		return;
	if (isFinished == true)
	{
		//vx = 0;
		//vy = 0;
		//x = start_x;
		//y = start_y;
		return;
	}	
	if (start_x - DISTANCE_DX_TO_DISAPPEAR > x
		|| start_x + DISTANCE_DX_TO_DISAPPEAR < x)
	{
		vx = 0;
		vy = FRAGMENT_SPEED_VY;

		//isFinished = true;

	}
	CGameObject::Update(dt, coObjects);
	
	x += dx;
	y += dy;
}
void CFragment::Render()
{
	if (isUsed == false)
		return;
	if (isFinished == true)
		return;
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CFragment::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FRAGMENT_BBOX_WIDTH;
	b = y + FRAGMENT_BBOX_HEIGHT;
}
CFragment::~CFragment()
{

}