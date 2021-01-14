#include"HiddenDoor.h"
CHiddenDoor::CHiddenDoor(float x, float y, int width, int height)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->type = OBJECT_TYPE_HIDDENDOOR;
	this->UpOrDown = false;
	this->Mario = NULL;
}
void CHiddenDoor::Render()
{
	RenderBoundingBox();
}
void CHiddenDoor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario == NULL)
		return;
	CGameObject::Update(dt);
	if (AABBCheck(Mario) == true)
	{
		if (UpOrDown == false)
		{
			if (Mario->isDucking == true)
			{
				Mario->isCrossingPipe = true;
				Mario->SetSpeed(0, MARIO_RESIST_GRAVITY);
			}
				
		}
		else
		{
			if (Mario->GetState() == MARIO_STATE_JUMP)
			{
				Mario->isCrossingPipe = true;
				Mario->SetSpeed(0, -MARIO_RESIST_GRAVITY);
			}
				
		}
		
	}
}
CHiddenDoor::~CHiddenDoor()
{
}
void CHiddenDoor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}