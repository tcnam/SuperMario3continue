#pragma once
#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "WeakBrick.h"


CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_SMALL;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	isOnGround = false;
	isTransform = false;

	isHoldingKoopas = false;
	isFlyFall = false;
	isFlying = false;
	isAttacking = false;
	isSliding = false;
	isDucking = false;
	isCrossingPipe = false;

	isRunningLeft = false;
	isRunningFastRight = false;
	isRunningRight = false;
	isRunningFastLeft = false;

	isClearingCourse = false;
	isInsidePlayScence = false;
	//allowJump = true;
	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}


void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	
	if (vy > 0.04f)
		isOnGround = false;
	if (GetTickCount64() - Transform_start > MARIO_TRANSFORM_TIME)
	{
		isTransform = false;
		Transform_start = 0;
		if(effect!=NULL)					//Pvevent error when switch scene
			effect->SetPosition(EFFECT_LOCATION_X_TO_HIDE, EFFECT_LOCATION_Y_TO_HIDE);
	}
	else
		return;
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (isCrossingPipe == true)
	{
		y += dy;
		return;
	}
		
	// Simple fall down
	if (isFlyFall == true)
		vy = MARIO_RESIST_GRAVITY;
	else if (isInsidePlayScence == false)
		vy = 0;
	else
		vy += MARIO_GRAVITY*dt;
	if (isClearingCourse == true)
	{
		vx = MARIO_AUTO_WALKING_SPEED;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

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
		if (nx != 0)
		{			
			vx = 0;
		}
		if (isInsidePlayScence == true)
		{
			if (ny < 0)
			{
				vy = 0;
				isFlyFall = false;
				if (state != MARIO_STATE_RUNNINGFAST_LEFT && state != MARIO_STATE_RUNNINGFAST_RIGHT)
				{
					isRunningFastLeft = isRunningFastRight = false;
					//isRunningLeft = isRunningRight = false;
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
				else if (dynamic_cast<CWeakBrick*>(e->obj))
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
		else
		{
			if (ny != 0)
				vy = 0;
		}		
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	if (tail != NULL)
	{
		if (tail->isUsed == false)
			tail->SetPosition(x, y);
	}
}

void CMario::Render()
{
	if (isTransform == true)
		return;
	int alpha = 255;
	if (untouchable) alpha = 128;
	if (isInsidePlayScence == false)
	{
		animation_set->at(0)->Render(round(x), round(y), alpha);
	}
	else
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
					if (nx > 0)
						ani = MARIO_ANI_BIG_HOLDKOOPAS_IDLE_RIGHT;
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
						if (isFlyFall == true)
						{
							if (nx > 0)
								ani = MARIO_ANI_TAIL_FALL_FLYRIGHT;
							else
								ani = MARIO_ANI_TAIL_FALL_FLYLEFT;
						}
						else if (state == MARIO_STATE_CHANGELEFT)
							ani = MARIO_ANI_TAIL_CHANGE_LEFT;
						else if (state == MARIO_STATE_CHANGERIGHT)
							ani = MARIO_ANI_TAIL_CHANGE_RIGHT;

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
					if (isFlyFall == true)
					{
						ani = MARIO_ANI_TAIL_FALL_FLYLEFT;
					}
					else if (isHoldingKoopas == true)
					{
						ani = MARIO_ANI_TAIL_HOLDKOOPAS_WALK_LEFT;
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
				if (isFlying == true)
				{
					if (nx > 0)
						ani = MARIO_ANI_TAIL_FLYRIGHT;
					else
						ani = MARIO_ANI_TAIL_FLYLEFT;
				}
				else if (isHoldingKoopas == true)
				{
					if (nx > 0)
						ani = MARIO_ANI_TAIL_HOLDKOOPAS_IDLE_RIGHT;
					else
						ani = MARIO_ANI_TAIL_HOLDKOOPAS_IDLE_LEFT;
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

					else if (isAttacking == true)
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
		if (isCrossingPipe == true)
		{
			ani = MARIO_ANI_CROSS_PIPE;
		}
		if (state == MARIO_STATE_DIE)
			ani = MARIO_ANI_DIE;
		animation_set->at(ani)->Render(round(x), round(y), alpha);
	}
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
	case MARIO_STATE_IDLE: 
		Stop();
		break;	
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		vx = 0;
		break;
	case MARIO_STATE_ATTACK:
		DebugOut(L"Attack\n");
		Attack();
		break;
	case MARIO_STATE_MOVE_UP:
		vy = -MARIO_WALKING_SPEED;
		break;
	case MARIO_STATE_MOVE_DOWN:
		vy = MARIO_WALKING_SPEED;
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
	if (isInsidePlayScence == false)
	{
		left = x;
		top = y;
		right = x + MARIO_WORLDSCENCE_BBOX_WIDTH;
		bottom = y + MARIO_WORLDSCENCE_BBOX_HEIGHT;
	}
	else
	{
		if (level == MARIO_LEVEL_BIG)
		{
			left = x;
			top = y;
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		else if (level == MARIO_LEVEL_SMALL)
		{
			left = x;
			top = y;
			right = x + MARIO_SMALL_BBOX_WIDTH;
			bottom = y + MARIO_SMALL_BBOX_HEIGHT;
		}
		else if (level == MARIO_LEVEL_TAIL)
		{
			if (isAttacking == true||state==MARIO_STATE_CHANGERIGHT||state==MARIO_STATE_CHANGELEFT)
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
	/*if (abs(vx) >= MARIO_WALKING_SPEED && abs(vx) < MARIO_RUNNING_SPEED&&isOnGround==true)
	{
		vx = abs(vx) + 0.01f;
	}
	if (abs(vx) >= MARIO_RUNNING_SPEED)
		vx = MARIO_RUNNING_SPEED;
	if(abs(vx)<MARIO_WALKING_SPEED)
		vx = MARIO_WALKING_SPEED;
	vx = vx * nx;
	DebugOut(L"speed:%f" , vx);*/
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
		tail->isUsed = true;
		
		if (nx > 0)
		{
			tail->SetStartPosition(x, y + 20.0f);
			tail->SetPosition(x, y+20.0f);
			tail->SetSpeed(TAIL_SPEED, 0);
		}
		else
		{
			tail->SetStartPosition(x + MARIO_BIG_BBOX_WIDTH, y + 20.0f);
			tail->SetPosition(x+MARIO_BIG_BBOX_WIDTH, y + 20.0f);
			tail->SetSpeed(-TAIL_SPEED, 0);
		}
	}
	else
	{
		DebugOut(L"can't attack");
	}
	
	DebugOut(L"Fire ball was created\n");
}


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
	//SetPosition(2264.00f, 64.00f);
	SetPosition(2480.00f, 80.00f);
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
void CMario::GoHiddenDoor()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_TAIL);
	SetPosition(2256.0f, -368.0f);
	SetSpeed(0, 0);
}
void CMario::GoToRunWay()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_TAIL);
	SetPosition(1152.0f, -48.0f);
	SetSpeed(0, 0);
}
CMario::~CMario()
{

}

