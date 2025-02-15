#include "Coin.h"

CCoin::CCoin()
{
	isFinished = false;
	Mario = NULL;
	isInsideWeakBrick = false;
}
void CCoin::Render()
{
	if (isInsideWeakBrick == true)
		return;
	if (isFinished == true)
		return;
	else
		animation_set->at(0)->Render(round(x), round(y));
}
void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (Mario == NULL)
		return;
	if (isFinished == true)
	{
		return;
	}
		
	if (isInsideWeakBrick == true)
		return;
	CGameObject::Update(dt, coObjects);
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (AABBCheck(Mario) == true)
	{
		CGame::GetInstance()->SetCoins(CGame::GetInstance()->GetCoins() + 1);
		CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
		isFinished = true;
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
			CGame::GetInstance()->SetCoins(CGame::GetInstance()->GetCoins() + 1);
			CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
			isFinished = true;
		}
			
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}
CCoin::~CCoin()
{

}