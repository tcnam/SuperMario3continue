#include"AutoDoor.h"
CAutoDoor::CAutoDoor(float x, float y, int width, int height,float des_x, float des_y)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->destination_x = des_x;
	this->destination_y = des_y;
	this->type = OBJECT_SYPE_AUTODOOR;
	this->Mario = NULL;
}
void CAutoDoor::Render()
{
	//RenderBoundingBox();
}
void CAutoDoor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario == NULL)
		return;
	CGameObject::Update(dt);
	if (AABBCheck(Mario) == true)
	{		
		Mario->SetPosition(destination_x, destination_y);
		Mario->isCrossingPipe = false;
		//Mario->GoUnderGround();
		//CGame::GetInstance()->SetCamPos(2128.0f, 64.0f);
	}
}
CAutoDoor::~CAutoDoor()
{
}
void CAutoDoor::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}