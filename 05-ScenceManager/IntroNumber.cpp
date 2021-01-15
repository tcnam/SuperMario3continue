#include "IntroNumber.h"

void CIntroNumber::Render()
{
	animation_set->at(0)->Render(round(x), round(y));
	//RenderBoundingBox();
}

void CIntroNumber::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	
}