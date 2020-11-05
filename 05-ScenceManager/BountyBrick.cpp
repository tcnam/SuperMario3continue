#include "BountyBrick.h"
CBountyBrick::CBountyBrick() :CGameObject()
{
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