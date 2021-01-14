#include "Portal.h"


CPortal::CPortal(float l, float t, float r, float b, int scene_id )
{
	this->scene_id = scene_id;
	x = l; 
	y = t;
	width =int( r - l + 1);
	height = int(b - t + 1);
	Mario = NULL;
}
void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario == NULL)
		return;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	CGameObject::Update(dt);
	if (AABBCheck(Mario) == true)
	{
		if (Mario->isInsidePlayScence == false)
		{
			if (Mario->GetState() ==MARIO_STATE_GOTO_ANOTHER_SCENCE)
			{
				CGame::GetInstance()->SwitchScene(scene_id);
			}
		}
		else
		{
			CGame::GetInstance()->SwitchScene(scene_id);
		}
	}
	//if (Mario->isInsidePlayScence == true)
	//{		
	//	CalcPotentialCollisions(coObjects, coEvents);
	//	if (coEvents.size() != 0)
	//	{
	//		float min_tx, min_ty, nx = 0, ny = 0;
	//		float rdx = 0;
	//		float rdy = 0;
	//		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
	//		if (nx != 0 || ny != 0)
	//		{
	//			CGame::GetInstance()->SwitchScene(scene_id);
	//		}
	//	}
	//}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
void CPortal::Render()
{
	RenderBoundingBox();
}
void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}
CPortal::~CPortal()
{

}