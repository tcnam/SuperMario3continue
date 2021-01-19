#pragma once
#include"FlyBrick.h"
CFlyBrick::CFlyBrick()
{
	SetState(FLYBRICK_STATE_VX);
	UpdateOrNot = false;
}
CFlyBrick::~CFlyBrick()
{

}
void CFlyBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float cam_x;
	float cam_y;
	CGame::GetInstance()->GetCamPos(cam_x, cam_y);
	if (x > cam_x + SCREEN_WIDTH)
		UpdateOrNot = false;
	else
		UpdateOrNot = true;
	if (UpdateOrNot == false)
		return;
	CGameObject::Update(dt);
	if (state == FLYBRICK_STATE_VY)
		vy += FLYBRICK_GRAVITY * dt;
	x += dx;
	y += dy;
}
void CFlyBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch ( state)
	{
	case FLYBRICK_STATE_VX:
		vx = -FLYBRICK_VX;
		vy = 0;
		break;
	case FLYBRICK_STATE_VY:
		vx = 0;
		//vy = FLYBRICK_VY;
		break;
	}

}
void CFlyBrick::Render()
{
	if (UpdateOrNot == false)
		return;
	animation_set->at(0)->Render(round(x), round(y));
}
void CFlyBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FLYBRICK_BBOX_WIDTH;
	b = y + FLYBRICK_BBOX_HEIGHT;
}