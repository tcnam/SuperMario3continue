#include "Coin.h"

CCoin::CCoin()
{
	isFinished = false;
}
void CCoin::Render()
{
	if (isFinished == true)
		return;
	else
		animation_set->at(0)->Render(x, y);
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}