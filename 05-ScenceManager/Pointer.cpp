#include "Pointer.h"

void CPointer::Render()
{
	animation_set->at(0)->Render(round(x), round(y));
	//RenderBoundingBox();
}

void CPointer::GetBoundingBox(float& l, float& t, float& r, float& b)
{

}