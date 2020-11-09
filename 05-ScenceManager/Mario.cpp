#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Koopas.h"
#include "Portal.h"
#include "Brick.h"
#include "BountyBrick.h"
#include "HiddenObject.h"


CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_TAIL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	isOnGround = true;
	isFlying = false;
	//allowJump = true;
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}


void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	vy += MARIO_GRAVITY*dt;
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
		//isOnGround = false;
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
		if (ny < 0)
		{
			vy = 0;
			if (!isOnGround)
			{
				isOnGround = true;
				if (state != MARIO_STATE_RUNNINGFAST_LEFT && state != MARIO_STATE_RUNNINGFAST_RIGHT)
				{
					isRunningFastLeft = isRunningFastRight = false;
					isFlying = false;
				}
			}
				
		}
		/*else
		{
			isOnGround = false;
		}*/
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
			else if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
				if (e->ny < 0)
				{
					if (koopas->GetState() != KOOPAS_STATE_DEFENSE_STATIC)
					{
						koopas->SetState(KOOPAS_STATE_DEFENSE_STATIC);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
					else
					{
						koopas->SetState(KOOPAS_STATE_DEFENSE_DYNAMIC);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
						
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (koopas->GetState() != KOOPAS_STATE_DEFENSE_STATIC)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
						else 
						{							
							koopas->SetState(KOOPAS_STATE_DEFENSE_DYNAMIC);
							koopas->vx = koopas->vx * nx;
						}
					}
				}
			}
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
						float x, y;
						bountybrick->GetPosition(x, y);
						bountybrick->SetState(BOUNTYBRICK_STATE_EMPTY);
						bountybrick->SetPosition(round(x), round(y -5));
					}
				}


			}
			else if (dynamic_cast<CHiddenObject*>(e->obj))
			{
				if (e->ny > 0)
				{
					y += dy;
					x += dx;
				}
				if (e->nx != 0)
				{
					x += dx;
					y += dy;
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->ny > 0)
					vy = 0;
			}
		}
	}
	//DebugOut(L"--> %s\n", ToWSTR(nx.ToString());

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani = -1;
	
	if (level == MARIO_LEVEL_BIG)					//big
	{
		if (vy >= 0)
		{
			if (vx == 0)
			{
				if (state == MARIO_STATE_CHANGELEFT)
					ani = MARIO_ANI_BIG_CHANGE_LEFT;
				else if (state == MARIO_STATE_CHANGERIGHT)
					ani = MARIO_ANI_BIG_CHANGE_RIGHT;

				else if (state == MARIO_STATE_WALKING_RIGHT)
					ani = MARIO_ANI_BIG_WALKING_RIGHT;
				else if (state == MARIO_STATE_RUNNING_RIGHT)
					ani = MARIO_ANI_BIG_WALKING_RIGHT;
				else if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
					ani = MARIO_ANI_BIG_WALKING_RIGHT;

				else if (state == MARIO_STATE_WALKING_LEFT)
					ani = MARIO_ANI_BIG_WALKING_LEFT;
				else if (state == MARIO_STATE_RUNNING_LEFT)
					ani = MARIO_ANI_BIG_WALKING_LEFT;
				else if (state == MARIO_STATE_RUNNINGFAST_LEFT)
					ani = MARIO_ANI_BIG_WALKING_LEFT;

				else
				{
					if (nx > 0)
						ani = MARIO_ANI_BIG_IDLE_RIGHT;
					else
						ani = MARIO_ANI_BIG_IDLE_LEFT;
				}
			}
			else if (vx > 0)
			{
				if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
					ani = MARIO_ANI_BIG_RUNNING_RIGHT;
				else
					ani = MARIO_ANI_BIG_WALKING_RIGHT;

			}
			else
			{
				if (state == MARIO_STATE_RUNNINGFAST_LEFT)
					ani = MARIO_ANI_BIG_RUNNING_LEFT;
				else
					ani = MARIO_ANI_BIG_WALKING_LEFT;
			}
		}
		else
		{
			if (nx > 0)
				ani = MARIO_ANI_BIG_JUMP_RIGHT;
			else
				ani = MARIO_ANI_BIG_JUMP_LEFT;			
		}
	}
	
	else if (level == MARIO_LEVEL_SMALL)				//small
	{
		if (vy >= 0)
		{
			if (vx == 0)
			{
				if (state == MARIO_STATE_CHANGELEFT)
					ani = MARIO_ANI_SMALL_CHANGE_LEFT;
				else if (state == MARIO_STATE_CHANGERIGHT)
					ani = MARIO_ANI_SMALL_CHANGE_RIGHT;

				else if (state == MARIO_STATE_WALKING_RIGHT)
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				else if (state == MARIO_STATE_RUNNING_RIGHT)
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				else if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;

				else if (state == MARIO_STATE_WALKING_LEFT)
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
				else if (state == MARIO_STATE_RUNNING_LEFT)
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
				else if (state == MARIO_STATE_RUNNINGFAST_LEFT)
					ani = MARIO_ANI_SMALL_WALKING_LEFT;

				else
				{
					if (nx > 0)
						ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					else
						ani = MARIO_ANI_SMALL_IDLE_LEFT;
				}
			}
			else if (vx > 0)
			{
				if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
					ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
				else
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;

			}
			else
			{
				if (state == MARIO_STATE_RUNNINGFAST_LEFT)
					ani = MARIO_ANI_SMALL_RUNNING_LEFT;
				else
					ani = MARIO_ANI_SMALL_WALKING_LEFT;
			}
		}
		else
		{
			if (nx > 0)
				ani = MARIO_ANI_SMALL_JUMP_RIGHT;
			else
				ani = MARIO_ANI_SMALL_JUMP_LEFT;
		}
	}
	else if (level == MARIO_LEVEL_TAIL)					//tail
	{
		if (vy >= 0)
		{
			if (vx == 0)
			{				
				if (state == MARIO_STATE_CHANGELEFT)
					ani = MARIO_ANI_TAIL_CHANGE_LEFT;
				else if (state == MARIO_STATE_CHANGERIGHT)
					ani = MARIO_ANI_TAIL_CHANGE_RIGHT;

				else if (state == MARIO_STATE_WALKING_RIGHT)
					ani = MARIO_ANI_TAIL_WALKING_RIGHT;
				else if(state==MARIO_STATE_RUNNING_RIGHT)
					ani= MARIO_ANI_TAIL_WALKING_RIGHT;
				else if (state==MARIO_STATE_RUNNINGFAST_RIGHT)
					ani = MARIO_ANI_TAIL_WALKING_RIGHT;

				else if (state == MARIO_STATE_WALKING_LEFT)
					ani = MARIO_ANI_TAIL_WALKING_LEFT;
				else if (state == MARIO_STATE_RUNNING_LEFT)
					ani = MARIO_ANI_TAIL_WALKING_LEFT;
				else if (state == MARIO_STATE_RUNNINGFAST_LEFT)
					ani = MARIO_ANI_TAIL_WALKING_LEFT;

				else
				{
					if (nx > 0)
						ani = MARIO_ANI_TAIL_IDLE_RIGHT;
					else
						ani = MARIO_ANI_TAIL_IDLE_LEFT;
				}
			}
			else if (vx > 0)
			{
				if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
					ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
				else 
					ani = MARIO_ANI_TAIL_WALKING_RIGHT;
				
			}
			else
			{
				if (state == MARIO_STATE_RUNNINGFAST_LEFT)
					ani = MARIO_ANI_TAIL_RUNNING_LEFT;
				else 
					ani = MARIO_ANI_TAIL_WALKING_LEFT;
			}
		}
		else
		{	
			if (isFlying==true)
			{
				if (nx > 0)
					ani = MARIO_ANI_TAIL_FLYRIGHT;
				else
					ani = MARIO_ANI_TAIL_FLYLEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_ANI_TAIL_JUMP_RIGHT;
				else
					ani = MARIO_ANI_TAIL_JUMP_LEFT;
			}
		}
	}
	else if (level == MARIO_LEVEL_FIRE)					//fire
	{
	if (vy >= 0)
	{
		if (vx == 0)
		{
			if (state == MARIO_STATE_CHANGELEFT)
				ani = MARIO_ANI_FIRE_CHANGE_LEFT;
			else if (state == MARIO_STATE_CHANGERIGHT)
				ani = MARIO_ANI_FIRE_CHANGE_RIGHT;

			else if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else if (state == MARIO_STATE_RUNNING_RIGHT)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;
			else if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;

			else if (state == MARIO_STATE_WALKING_LEFT)
				ani = MARIO_ANI_FIRE_WALKING_LEFT;
			else if (state == MARIO_STATE_RUNNING_LEFT)
				ani = MARIO_ANI_FIRE_WALKING_LEFT;
			else if (state == MARIO_STATE_RUNNINGFAST_LEFT)
				ani = MARIO_ANI_FIRE_WALKING_LEFT;

			else
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else
					ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
		}
		else if (vx > 0)
		{
			if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
				ani = MARIO_ANI_FIRE_RUNNING_RIGHT;
			else
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;

		}
		else
		{
			if (state == MARIO_STATE_RUNNINGFAST_LEFT)
				ani = MARIO_ANI_FIRE_RUNNING_LEFT;
			else
				ani = MARIO_ANI_FIRE_WALKING_LEFT;
		}
	}
	else
	{
		if (nx > 0)
			ani = MARIO_ANI_FIRE_JUMP_RIGHT;
		else
			ani = MARIO_ANI_FIRE_JUMP_LEFT;
	}
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
		DebugOut(L"walking right\n");
		Right();
		Walk();
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		DebugOut(L"running right\n");
		Right();
		Run();
		break;
	case MARIO_STATE_RUNNINGFAST_RIGHT:
		Right();
		RunFast();
		DebugOut(L"running fast right\n");
		break;
	case MARIO_STATE_FLYRIGHT:
		DebugOut(L"fly right\n");
		Fly();
		break;
	case MARIO_STATE_WALKING_LEFT: 
		DebugOut(L"walking left\n");
		Left();
		Walk();
		break;	
	case MARIO_STATE_RUNNING_LEFT:
		DebugOut(L"running left\n");
		Left();
		DebugOut(L"vx: %f\n", vx);
		Run();
		break;	
	case MARIO_STATE_RUNNINGFAST_LEFT:
		DebugOut(L"running fast left\n");
		Left();
		RunFast();
		break;
	case MARIO_STATE_FLYLEFT:
		DebugOut(L"fly left\n");
		Fly();
		break;
	case MARIO_STATE_CHANGERIGHT:
		ChangeDirectionRight();
		break;
	case MARIO_STATE_CHANGELEFT:
		ChangeDirectionLeft();
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		DebugOut(L"Jump\n");
		Jump();
		break;
	case MARIO_STATE_FLYFALL:
		Fall();
		break;
	/*case MARIO_STATE_HIGHJUMP:
		JumpHigh();
		break;*/
	case MARIO_STATE_IDLE: 
		Stop();
		break;
	
	
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}
void CMario::GetBoundingBoxTailLevel(float& left, float& top, float& right, float& bottom)
{
	if (nx > 0)
	{
		left = x + 7;
		right = left + MARIO_TAIL_BBOX_WIDTH;
	}		
	else
	{
		left = x;
		right = x + MARIO_TAIL_BBOX_WIDTH;
	}	
	top = y;
	bottom = y + MARIO_TAIL_BBOX_HEIGHT;
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		left = x;
		top = y;
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if(level==MARIO_LEVEL_SMALL)
	{
		left = x;
		top = y;
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TAIL)
	{
		CMario::GetBoundingBoxTailLevel(left, top, right, bottom);
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		left = x;
		top = y;
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
}
void CMario::Stop()
{
	vx = 0;
}
void CMario::Left()
{
	nx = -1;
}
void CMario::Right()
{
	nx = 1;
}
void CMario::Walk()
{
	vx = MARIO_WALKING_SPEED * nx;
}
void CMario::Run()
{
	vx = MARIO_RUNNING_SPEED * nx;
}
void CMario::RunFast()
{
	vx = MARIO_RUNNINGFAST_SPEED * nx;
}
void CMario::ChangeDirectionRight()
{
	nx = 1;
}
void CMario::ChangeDirectionLeft()
{
	nx = -1;
}
void CMario::Jump()
{
	//if (allowJump == false)
		//return;
	if (isOnGround ==false)//allow mario to jump only when on ground
		return;
	vy =- MARIO_JUMP_SPEED_Y;
	isOnGround = false;
}
void CMario::Fly()
{
	vy = -MARIO_FLY_SPEED_Y;
	vx = MARIO_RUNNING_SPEED * nx;
	isOnGround = false;
}
void CMario::Fall()
{
	isOnGround = false;
	vy += MARIO_RESIST_GRAVITY * dt;
	vx = MARIO_WALKING_SPEED * nx;
}
/*void CMario::JumpHigh()
{
	//if (allowJump == false)
		//return;
	if (isOnGround == false)//allow mario to jump only when on ground
		return;
	vy = -MARIO_JUMPHIGH_SPEED_Y;
	isOnGround = false;
}*/


/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_TAIL);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

