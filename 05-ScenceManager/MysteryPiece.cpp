#include "MysteryPiece.h"

CMysteryPiece::CMysteryPiece()
{
	isCollidedWithMario = false;
	isFinished = false;
	Mario = NULL;
	CGameObject::SetState(-1);
	t = 0;
}
void CMysteryPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isFinished == true)
		return;
	CGameObject::Update(dt);
	if (isCollidedWithMario == true)
	{
		vy = -MYSTERYPIECE_FLY_SPEED;
		y += dy;
		if (y < -MYSTERYPIECE_Y_LIMIT)
		{
			CGame::GetInstance()->SetStatesOfMyPieces(state);
			isFinished = true;
		}			
	}
	else
	{
		if (t == 0)
			t = (DWORD)GetTickCount64();
		if (GetTickCount64() - t > onesec && isCollidedWithMario==false)
		{
			t = 0;
			if (state == -1)
				return;
			if (state == MYSTERYPIECE_STATE_STAR)
				state = 0;
			else
				state++;
		}
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();
		if (AABBCheck(Mario) == true)
		{
			isCollidedWithMario = true;
			Mario->isClearingCourse = true;
		}
		CalcPotentialCollisions(coObjects, coEvents);
		if (coEvents.size() != 0)
		{
			float min_tx, min_ty, nx = 0, ny = 0;
			float rdx = 0;
			float rdy = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			if (nx != 0 || ny != 0)
			{
				isCollidedWithMario = true;
				Mario->isClearingCourse = true;
			}
		}
	}	
}
void CMysteryPiece::Render()
{
	if (state == MYSTERYPIECE_STATE_MUSHROOM)
		animation_set->at(MYSTERYPIECE_STATE_MUSHROOM)->Render(round(x), round(y));
	else if (state == MYSTERYPIECE_STATE_FLOWER)
		animation_set->at(MYSTERYPIECE_STATE_FLOWER)->Render(round(x), round(y));
	else if (state == MYSTERYPIECE_STATE_STAR)
		animation_set->at(MYSTERYPIECE_STATE_STAR)->Render(round(x), round(y));
	//else
	//	return;
}
void CMysteryPiece::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MYSTERYPIECE_BBOX_WIDTH;
	b = y + MYSTERYPIECE_BBOX_HEIGHT;
}
CMysteryPiece::~CMysteryPiece()
{

}
