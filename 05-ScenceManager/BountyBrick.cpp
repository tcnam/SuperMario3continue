#include "BountyBrick.h"
CBountyBrick::CBountyBrick() :CGameObject()
{
	vx = 0;
	vy = 0;
	SetState(BOUNTYBRICK_STATE_NORMAL);
}
void CBountyBrick::Render()
{
	if (state == BOUNTYBRICK_STATE_EMPTY)
		animation_set->at(1)->Render(x, y);		//empty bounty brick animation
	else if(state==BOUNTYBRICK_STATE_NORMAL)
		animation_set->at(0)->Render(x, y);		//normal bounty brick animation
	//RenderBoundingBox();
}
void CBountyBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == BOUNTYBRICK_STATE_NORMAL)
		return;
	CGameObject::Update(dt, coObjects);
	if (y <= start_y - BOUNTYBRICK_MOVING_DISTANCE_TOBACK)
	{
		vy = BOUNTYBRICK_SPEED_Y;
	}
	else if (y > start_y)
	{
		y = start_y;
		vy = 0;
	}
	y += dy;
}

void CBountyBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}
void CBountyBrick::SetState(int state)
{
	CGameObject::SetState(state);
}