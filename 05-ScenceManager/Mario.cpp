#pragma once
#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Portal.h"


CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	isOnGround = false;

	isHoldingKoopas = false;
	isFlyFall = false;
	isFlying = false;
	isAttacking = false;
	isSliding = false;

	isRunningLeft = false;
	isRunningFastRight = false;
	isRunningRight = false;
	isRunningFastLeft = false;
	//allowJump = true;
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}


void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (vy > 0)
		isOnGround = false;
	// Calculate dx, dy 
	CGameObject::Update(dt);
	// Simple fall down
	if (isFlyFall == true)
		vy = MARIO_RESIST_GRAVITY;
	else
		vy += MARIO_GRAVITY*dt;
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	//if (GetTickCount64() - Slide_start > MARIO_SLDIE_TIME)
	//{
	//	Slide_start = 0;
	//	isSliding = false;
	//}
	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = false;
	}
	if (level == MARIO_LEVEL_TAIL)
	{
		if (GetTickCount64() - Attack_start > MARIO_ATTACK_TIME_TAIL)
		{
			isAttacking = false;
			Attack_start = 0;
		}
		if (GetTickCount64() - FlyFall_start > MARIO_FLYFALL_TIME)
		{
			isFlyFall = false;
			FlyFall_start = 0;
		}
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		if (GetTickCount64() - Attack_start > MARIO_ATTACK_TIME_FIRE)
		{
			isAttacking = false;
			Attack_start = 0;
		}
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
	/*	if (rdx != 0 && rdx!=dx)
			x += nx*abs(rdx); */
			// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (nx != 0) vx = 0;
		if (ny < 0)
		{
			vy = 0;
			isFlyFall = false;
			if (state != MARIO_STATE_RUNNINGFAST_LEFT && state != MARIO_STATE_RUNNINGFAST_RIGHT)
			{
				isRunningFastLeft = isRunningFastRight = false;
				isFlying = false;
			}
			if (!isOnGround)
			{
				isOnGround = true;
			}
		}

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CHiddenObject*>(e->obj))
			{
				if (ny > 0)
				{
					//if (isOnGround == false)				//if mario was blocked by hidden object vy=0 so we need to set vy back to its value
					//	vy = -MARIO_JUMP_SPEED_Y;
					y += dy;
				}
				if (ny < 0)
				{
					isOnGround = true;
					vy = 0;
				}
				if (nx != 0)
				{
					x += min_tx * dx + nx * 0.4f;
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				if (nx != 0)
				{
					vx = 0;
				}
				if (ny < 0)
				{
					isOnGround = true;
					vy = 0;
				}
			}
		}
		
	}
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
				if (isHoldingKoopas == true)
				{
					if (nx > 0)
						ani = MARIO_ANI_BIG_HOLDKOOPAS_IDLE_RIGHT;
					else
						ani = MARIO_ANI_BIG_HOLDKOOPAS_IDLE_LEFT;
				}
				else
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
				
			}
			else if (vx > 0)
			{
				if (isHoldingKoopas == true)
				{
					ani = MARIO_ANI_BIG_HOLDKOOPAS_WALK_RIGHT;
				}
				else
				{
					if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
						ani = MARIO_ANI_BIG_RUNNING_RIGHT;
					else
						ani = MARIO_ANI_BIG_WALKING_RIGHT;
				}
				

			}
			else
			{
				if (isHoldingKoopas == true)
				{
					ani = MARIO_ANI_BIG_HOLDKOOPAS_WALK_LEFT;
				}
				else
				{
					if (state == MARIO_STATE_RUNNINGFAST_LEFT)
						ani = MARIO_ANI_BIG_RUNNING_LEFT;
					else
						ani = MARIO_ANI_BIG_WALKING_LEFT;
				}
				}
				
		}
		else
		{
			if (isHoldingKoopas == true)
			{
				if(nx>0)
					ani= MARIO_ANI_BIG_HOLDKOOPAS_IDLE_RIGHT;
				else
					ani = MARIO_ANI_BIG_HOLDKOOPAS_IDLE_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_ANI_BIG_JUMP_RIGHT;
				else
					ani = MARIO_ANI_BIG_JUMP_LEFT;
			}
					
		}
	}
	
	else if (level == MARIO_LEVEL_SMALL)				//small
	{
		if (vy >= 0)
		{
			if (vx == 0)
			{
				if (isHoldingKoopas == true)
				{
					if (nx > 0)
						ani = MARIO_ANI_SMALL_HOLDKOOPAS_IDLE_RIGHT;
					else
						ani = MARIO_ANI_SMALL_HOLDKOOPAS_IDLE_LEFT;
				}
				else
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
			}
			else if (vx > 0)
			{
				if (isHoldingKoopas == true)
				{
					ani = MARIO_ANI_SMALL_HOLDKOOPAS_WALK_RIGHT;
				}
				else
				{
					if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
						ani = MARIO_ANI_SMALL_RUNNING_RIGHT;
					else
						ani = MARIO_ANI_SMALL_WALKING_RIGHT;
				}
				

			}
			else
			{
				if (isHoldingKoopas == true)
				{
					ani = MARIO_ANI_SMALL_HOLDKOOPAS_WALK_LEFT;
				}
				else
				{
					if (state == MARIO_STATE_RUNNINGFAST_LEFT)
						ani = MARIO_ANI_SMALL_RUNNING_LEFT;
					else
						ani = MARIO_ANI_SMALL_WALKING_LEFT;
				}
			}
		}
		else
		{
			if (isHoldingKoopas == true)
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_HOLDKOOPAS_IDLE_RIGHT;
				else
					ani = MARIO_ANI_SMALL_HOLDKOOPAS_IDLE_LEFT;
			}
			else
			{
				if (nx > 0)
					ani = MARIO_ANI_SMALL_JUMP_RIGHT;
				else
					ani = MARIO_ANI_SMALL_JUMP_LEFT;
			}
		}
	}
	else if (level == MARIO_LEVEL_TAIL)					//tail
	{
		if (vy >= 0)
		{
			if (vx == 0)
			{			
				if (isHoldingKoopas == true)
				{
					if (nx > 0)
						ani = MARIO_ANI_TAIL_HOLDKOOPAS_IDLE_RIGHT;
					else
						ani = MARIO_ANI_TAIL_HOLDKOOPAS_IDLE_LEFT;
				}
				else
				{
					if (state == MARIO_STATE_CHANGELEFT)
						ani = MARIO_ANI_TAIL_CHANGE_LEFT;
					else if (state == MARIO_STATE_CHANGERIGHT)
						ani = MARIO_ANI_TAIL_CHANGE_RIGHT;

					else if (isFlyFall == true)			
					{
						if (nx > 0)
							ani = MARIO_ANI_TAIL_FALL_FLYRIGHT;
						else
							ani = MARIO_ANI_TAIL_FALL_FLYLEFT;
					}
					

					else if (state == MARIO_STATE_WALKING_RIGHT)
						ani = MARIO_ANI_TAIL_WALKING_RIGHT;
					else if (state == MARIO_STATE_RUNNING_RIGHT)
						ani = MARIO_ANI_TAIL_WALKING_RIGHT;
					else if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
						ani = MARIO_ANI_TAIL_WALKING_RIGHT;

					else if (state == MARIO_STATE_WALKING_LEFT)
						ani = MARIO_ANI_TAIL_WALKING_LEFT;
					else if (state == MARIO_STATE_RUNNING_LEFT)
						ani = MARIO_ANI_TAIL_WALKING_LEFT;
					else if (state == MARIO_STATE_RUNNINGFAST_LEFT)
						ani = MARIO_ANI_TAIL_WALKING_LEFT;
					else if (isAttacking == true)
					{
						if (nx > 0)
							ani = MARIO_ANI_TAIL_ATTACK_RIGHT;
						else
							ani = MARIO_ANI_TAIL_ATTACK_LEFT;
					}
					else
					{
						if (nx > 0)
							ani = MARIO_ANI_TAIL_IDLE_RIGHT;
						else
							ani = MARIO_ANI_TAIL_IDLE_LEFT;
					}
				}
			}
			else if (vx > 0)
			{		
				if (isHoldingKoopas == true)
				{
					ani = MARIO_ANI_TAIL_HOLDKOOPAS_WALK_RIGHT;
				}
				else if (isFlyFall == true)			
				{
					ani = MARIO_ANI_TAIL_FALL_FLYRIGHT;
				}
				else
				{
					if (state == MARIO_STATE_RUNNINGFAST_RIGHT)
						ani = MARIO_ANI_TAIL_RUNNING_RIGHT;
					else
						ani = MARIO_ANI_TAIL_WALKING_RIGHT;
				}				
			}
			else
			{
				if (isHoldingKoopas == true)
				{
					ani = MARIO_ANI_TAIL_HOLDKOOPAS_WALK_LEFT;
				}
				else if (isFlyFall == true)			
				{
					ani = MARIO_ANI_TAIL_FALL_FLYLEFT;
				}
				else
				{
					if (state == MARIO_STATE_RUNNINGFAST_LEFT)
						ani = MARIO_ANI_TAIL_RUNNING_LEFT;
					else
						ani = MARIO_ANI_TAIL_WALKING_LEFT;
				}
			}
		}
		else
		{	
			if (isHoldingKoopas == true)
			{
				if (nx > 0)
					ani = MARIO_ANI_TAIL_HOLDKOOPAS_IDLE_RIGHT;
				else
					ani = MARIO_ANI_TAIL_HOLDKOOPAS_IDLE_LEFT;
			}
			else if (isFlying==true)
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
			
			else if (isAttacking==true)
			{
				if (nx > 0)
					ani = MARIO_ANI_FIRE_ATTACK_RIGHT;
				else
					ani = MARIO_ANI_FIRE_ATTACK_LEFT;
			}
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

	RenderBoundingBox();
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
	/*case MARIO_STATE_SLIDE_RIGHT:
		DebugOut(L"Slide right\n");
		Right();		
		Slide();
		DebugOut(L"vx: %f\n", vx);
		break;*/
	
	case MARIO_STATE_WALKING_LEFT: 
		DebugOut(L"walking left\n");
		Left();
		Walk();
		break;	
	case MARIO_STATE_RUNNING_LEFT:
		DebugOut(L"running left\n");
		Left();
		//DebugOut(L"vx: %f\n", vx);
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
	/*case MARIO_STATE_SLIDE_LEFT:
		DebugOut(L"Slide left\n");
		Left();
		Slide();
		DebugOut(L"vx: %f\n", vx);
		break;*/
	
	case MARIO_STATE_CHANGERIGHT:
		DebugOut(L"Change right\n");
		Right();
		ChangeDirection();
		break;
	case MARIO_STATE_CHANGELEFT:
		DebugOut(L"Change left\n");
		Left();
		ChangeDirection();
		break;
	
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		DebugOut(L"Jump\n");
		Jump();
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
	case MARIO_STATE_ATTACK:
		DebugOut(L"Attack\n");
		Attack();
		break;


	}
}
void CMario::GetBoundingBoxTailLevel(float& left, float& top, float& right, float& bottom)
{
	if (nx > 0||vx>0)
	{
		left = x + 7;
		right = left + MARIO_TAIL_BBOX_WIDTH;
	}		
	else if(nx<0||vx<0)
	{
		left = x;
		right = x + MARIO_TAIL_BBOX_WIDTH;
	}	
	top = y;
	bottom = y + MARIO_TAIL_BBOX_HEIGHT;
}
void CMario::GetBoundingBoxTailWhileAttacking(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MARIO_TAIL_ATTACK_BBOX_WIDTH;
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
		if (isAttacking == true)
			GetBoundingBoxTailWhileAttacking(left, top, right, bottom);
		else
			GetBoundingBoxTailLevel(left, top, right, bottom);
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
void CMario::ChangeDirection()
{
	vx = 0;
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
//void CMario :: Slide()
//{
//	if (isSliding == false)
//		return;
//	else
//	{
//		if (isRunningLeft == true)
//			vx = MARIO_RUNNING_SPEED * nx;
//		else if (isRunningRight == true)
//			vx = MARIO_RUNNING_SPEED * nx;
//		else if(isRunningFastRight == true)
//			vx = MARIO_RUNNINGFAST_SPEED * nx;
//		else if (isRunningFastLeft == true)
//			vx = MARIO_RUNNINGFAST_SPEED * nx;
//		else
//			vx = MARIO_WALKING_SPEED * nx;
//	}
//	isRunningFastLeft = false;
//	isRunningFastRight = false;
//	isRunningLeft = false;
//	isRunningRight = false;
//}
void CMario::Attack()
{
	if (level == MARIO_LEVEL_FIRE)
	{
		if (fireballs == NULL)
		{
			DebugOut(L"There arn't no fire ball");
			return;
		}
		else
		{
			if (fireballs->isFinished == true)
			{
				if (nx > 0)
				{
					fireballs->SetPosition(x + FIREBALL_RELATIVE_POSITION_X, y + FIREBALL_RELATIVE_POSITION_Y);
					fireballs->SetSpeed(FIREBALL_SPEED, -FIREBALL_GRAVITY);
					//fireballs->SetDirectionnx(1);
				}
				else
				{
					fireballs->SetPosition(x - FIREBALL_RELATIVE_POSITION_X, y + FIREBALL_RELATIVE_POSITION_Y);
					fireballs->SetSpeed(-FIREBALL_SPEED, -FIREBALL_GRAVITY);
					//fireballs->SetDirectionnx(-1);
				}

				fireballs->isUsed = true;
				fireballs->isFinished = false;
			}
			
		}
	}
	else if (level == MARIO_LEVEL_TAIL)
	{
		
	}
	
	DebugOut(L"Fire ball was created\n");
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
void CMario::ResetTail()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_TAIL);
	SetPosition(1904.00f, -64.00f);
	SetSpeed(0, 0);
}
void CMario::ResetBig()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
void CMario::GoUnderGround()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(2272.00f, 176.00f);
	SetSpeed(0, 0);
}
void CMario::ResetSmall()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_SMALL);
	SetPosition(x, y-2);
	SetSpeed(0, 0);
}
void CMario::ResetFire()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_FIRE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}


