#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "Brick.h"
#include "BountyBrick.h"
#include "HiddenObject.h"


CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	previousstate = MARIO_STATE_IDLE;
	SetState(previousstate);
	isMoving = false;
	changeDirection = false;
	isOnGround = false;

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}
/*void CMario::CollisionWithBrick(vector<LPGAMEOBJECT>* coObject)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	vector<LPGAMEOBJECT> BrickObjects;
	BrickObjects.clear();
	for (UINT i = 0; i < coObject->size(); i++)
		if (coObject->at(i)->GetType() == OBJECT_TYPE_BRICK)
			BrickObjects.push_back(coObject->at(i));
	CalcPotentialCollisions(&BrickObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->ny < 0)
				CMario::isOnGround = true;
			//Check if mario was on ground
		}
	}
}
void CMario::CollisionWithBountyBrick(vector<LPGAMEOBJECT>* coObject)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	vector<LPGAMEOBJECT> BountyBrickObjects;
	BountyBrickObjects.clear();
	for (UINT i = 0; i < coObject->size(); i++)
		if (coObject->at(i)->GetType() == OBJECT_TYPE_BOUNTYBRICK)
			BountyBrickObjects.push_back(coObject->at(i));
	CalcPotentialCollisions(&BountyBrickObjects, coEvents);
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->ny < 0)
				CMario::isOnGround = true;
			else if (e->ny > 0)
			{
				if (e->obj->GetState() != BOUNTYBRICK_STATE_EMPTY)
					e->obj->SetState(BOUNTYBRICK_STATE_EMPTY);
			}
				
			
		}
	}
}*/

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	
	// Simple fall down
	vy += MARIO_GRAVITY*dt;
	/*if(CMario::GetState()==MARIO_STATE_JUMP)
		CollisionWithBountyBrick(coObjects);
	else
		CollisionWithBrick(coObjects);
	*/
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		// block every object first!
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny<0) vy = 0;	


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<CBountyBrick*>(e->obj))
			{
				CBountyBrick* bountybrick = static_cast<CBountyBrick*>(e->obj);
				if (e->ny > 0)
				{
					vy = 0;
					if (bountybrick->GetState() != BOUNTYBRICK_STATE_EMPTY)
					{
						bountybrick->SetState(BOUNTYBRICK_STATE_EMPTY);
					}
				}
				else if (e->ny < 0)
					CMario:isOnGround = true;

			}
			else if (dynamic_cast<CHiddenObject*>(e->obj))
			{
				if (e->ny < 0)
					CMario::isOnGround = true;
				else if (e->ny > 0)
				{
					y += dy;
					x += dx;
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny < 0)
					CMario::isOnGround = true;
			}//Check if mario was on ground
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	if (level == MARIO_LEVEL_BIG)
	{
			if (state==MARIO_STATE_IDLE && vx == 0 && vy >= 0)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else
					ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if /*(state==MARIO_STATE_RUNNING_RIGHT||state==MARIO_STATE_WALKING_RIGHT)*/(vy >= 0 && nx > 0)
				ani = MARIO_ANI_BIG_WALKING_RIGHT;
			else if /*(state == MARIO_STATE_RUNNING_LEFT || state == MARIO_STATE_WALKING_LEFT)*/(vy >= 0 && nx < 0)
				ani = MARIO_ANI_BIG_WALKING_LEFT;
			else if (state==MARIO_STATE_JUMP||CMario::isOnGround==false)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else
					ani = MARIO_ANI_BIG_JUMP_LEFT;
			}
			else if (changeDirection==true)
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_CHANGE_RIGHT;
				else
					ani = MARIO_ANI_BIG_CHANGE_LEFT;
			}
			else
				ani = MARIO_ANI_SMALL_IDLE_LEFT;
		
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		if (vx == 0)
		{
			if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
			else ani = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = MARIO_ANI_SMALL_WALKING_RIGHT;
		else ani = MARIO_ANI_SMALL_WALKING_LEFT;
	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		Right();
		Walk();
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		Right();
		Run();
		break;
	case MARIO_STATE_WALKING_LEFT: 
		Left();
		Walk();
		break;	
	case MARIO_STATE_RUNNING_LEFT:
		Left();
		Run();
		break;	
	case MARIO_STATE_CHANGEDIRECTION:
		ChangeDirection();
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		Jump();
		break;
	case MARIO_STATE_IDLE: 
		Stop();
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 

	if (level==MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
}
void CMario::Stop()
{
	vx = 0;
	//changeDirection = false;
}
void CMario::Left()
{
	nx = -1;
	//changeDirection = false;
}
void CMario::Right()
{
	nx = 1;
	//changeDirection = false;
}
void CMario::Walk()
{
	vx = MARIO_WALKING_SPEED * nx;
}
void CMario::Run()
{
	vx = MARIO_RUNNING_SPEED * nx;
}
void CMario::ChangeDirection()
{
	//changeDirection = true;
	nx = -nx;
}
void CMario::Jump()
{
	if (isOnGround ==false)//allow mario to jump only when on ground
		return;
	vy=-MARIO_JUMP_SPEED_Y;
	isOnGround = false;
}


/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

