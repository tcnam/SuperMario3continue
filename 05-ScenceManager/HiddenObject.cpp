#include"HiddenObject.h"
CHiddenObject::CHiddenObject(float x, float y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->type = OBJECT_TYPE_HIDDENOBJECT;
}
void CHiddenObject::Render()
{
	//RenderBoundingBox();
}
void CHiddenObject::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}
CHiddenObject::~CHiddenObject()
{
}
void CHiddenObject::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}