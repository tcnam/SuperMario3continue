#include "Koopas.h"

CKoopas::CKoopas()
{
	vy = 0;
	right();
	SetState(KOOPAS_STATE_WALKING);
	isWaiting = false;
	wait_start = 0;
	isOnGround = false;
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state != KOOPAS_STATE_WALKING)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (MarioMain == NULL)
		return;
	if (MarioMain->GetState() == MARIO_STATE_DIE)
		return;
	if (MarioMain->isTransform == true)
		return;
	if (vy > 0.03f)
		isOnGround = false;
	float Mario_x, Mario_y;
	MarioMain->GetPosition(Mario_x, Mario_y);
	if (state == KOOPAS_STATE_DEFENSE_STATIC || state == KOOPAS_STATE_KICKOUT_2||state==KOOPAS_STATE_ISHOLD)
	{
		if (level == KOOPAS_LEVEL_FLY)
			level = KOOPAS_LEVEL_NORMAL;
		if (wait_start != 0)
		{
			if (GetTickCount64() - wait_start > KOOPAS_TIME_WAIT_TO_TRANSFORM)
			{
				if (isHold == true)
				{
					MarioForHold = NULL;
					isHold = false;
					MarioMain->isHoldingKoopas = false;
					if (AABBCheck(MarioMain) == true)
					{
						if (MarioMain->GetLevel() == MARIO_LEVEL_SMALL)
							MarioMain->SetState(MARIO_STATE_DIE);
						else
						{
							MarioMain->StartUntouchable();
							if (MarioMain->GetEffect() != NULL)
							{
								MarioMain->GetEffect()->SetState(EFFECT_CLOUND);
								MarioMain->GetEffect()->SetPosition(Mario_x, Mario_y);
								MarioMain->StartTransForm();
							}
							if (MarioMain->GetLevel() == MARIO_LEVEL_FIRE || MarioMain->GetLevel() == MARIO_LEVEL_TAIL)
								MarioMain->SetLevel(MARIO_LEVEL_BIG);
							else
								MarioMain->SetLevel(MARIO_LEVEL_SMALL);
						}
					}
				}
				level = KOOPAS_LEVEL_NORMAL;
				state = KOOPAS_STATE_WALKING;
				vx = -KOOPAS_WALKING_SPEED;
				SetPosition(x, y - KOOPAS_BBOX_HEIGHT + KOOPAS_BBOX_HEIGHT_DIE);
			}
		}
	}
	else
	{
		if (wait_start != 0)
		{
			wait_start = 0;
			isWaiting = false;
		}
	}
	
	if (level == KOOPAS_LEVEL_FLY)
	{		
		if (abs(x - start_x) >= KOOPAS_DX_LIMIT_TOFLY)
		{
			vy = -KOOPAS_FLY_JUMP_SPEED;
			if (Mario_x < x)
				vx = -KOOPAS_WALKING_SPEED;
			else
				vx = KOOPAS_WALKING_SPEED;
			start_x = x;
		}
	}
	else if (level == KOOPAS_LEVEL_FLY2)
	{
		vx = 0;
		if(vy==0)
			vy = -KOOPAS_FLY2_MOVING_SPEED;
		if (y < start_y- KOOPAS_DY_LIMIT_TOCHANGE_VY)
		{
			vy = KOOPAS_FLY2_MOVING_SPEED;
		}
		else if (y >= start_y+ KOOPAS_DY_LIMIT_TOCHANGE_VY)
		{
			vy = -KOOPAS_FLY2_MOVING_SPEED;
		}
	}
	else
	{
		if (isOnGround == false)
		{
			if (state == KOOPAS_STATE_WALKING)
			{
				if (vx > 0)
				{
					SetPosition(x - 2, y - 1);
					vx = -KOOPAS_WALKING_SPEED;
				}
				else
				{
					SetPosition(x + 2, y - 1);
					vx = KOOPAS_WALKING_SPEED;
				}
			}
			
				
		}			
	}
	CGameObject::Update(dt, coObjects);
	if (isHold==true)
	{
		vx = 0;
		vy = 0;
		if (MarioForHold->isHoldingKoopas == true)
		{
			if (MarioForHold->GetLevel() != MARIO_LEVEL_SMALL)
			{
				if (MarioForHold->GetState() != MARIO_STATE_IDLE)//when mario in state idle its x location is smaller than in others state because mario is not moving
				{
					if (MarioForHold->Getnx() > 0)
					{
						if (MarioForHold->GetLevel() == MARIO_LEVEL_TAIL)
							SetPosition(Mario_x + 20.00f, Mario_y + 7.00f);
						else
							SetPosition(Mario_x + 13.00f, Mario_y + 7.00f);
					}
					else
						SetPosition(Mario_x - 14.00f, Mario_y + 7.00f);
				}
				else
				{
					if (MarioForHold->Getnx() > 0)
					{
						if (MarioForHold->GetLevel() == MARIO_LEVEL_TAIL)
							SetPosition(Mario_x + 18.00f, Mario_y + 7.00f);
						else
							SetPosition(Mario_x + 11.00f, Mario_y + 7.00f);
					}
					else
						SetPosition(Mario_x - 13.00f, Mario_y + 7.00f);
				}
			}
			else
			{
				if (MarioForHold->GetState() != MARIO_STATE_IDLE)//when mario in state idle its x location is smaller than in others state because mario is not moving
				{
					if (MarioForHold->Getnx() > 0)
					{

						SetPosition(Mario_x + 11.00f, Mario_y - 3.00f);
					}
					else
						SetPosition(Mario_x - 13.00f, Mario_y - 3.00f);
				}
				else
				{
					if (MarioForHold->Getnx() > 0)
					{

						SetPosition(Mario_x + 9.00f, Mario_y - 3.00f);
					}
					else
						SetPosition(Mario_x - 12.00f, Mario_y - 3.00f);
				}
			}
		}
		else
		{
			if (level == KOOPAS_LEVEL_FLY)
				level = KOOPAS_LEVEL_NORMAL;
			state = KOOPAS_STATE_DEFENSE_DYNAMIC;
			if (MarioForHold->Getnx() > 0)
				vx = KOOPAS_DYNAMIC_SPEED;
			else
				vx = -KOOPAS_DYNAMIC_SPEED;
			MarioForHold->isHoldingKoopas = false;
			MarioForHold = NULL;
			isHold = false;
		}

	}
	else if (MarioMain->isReadyToHold == true&&state==KOOPAS_STATE_DEFENSE_STATIC&&MarioMain->isHoldingKoopas==false&&isHold==false)
	{
		if (Mario_y<y + MARIO_SMALL_BBOX_HEIGHT + 1 && Mario_y>y - MARIO_SMALL_BBOX_HEIGHT)
		{
			if (MarioMain->nx > 0)
			{
				if (MarioMain->GetLevel() == MARIO_LEVEL_TAIL)
				{
					if (Mario_x > x - 32&&Mario_x<x)
					{
						MarioForHold = MarioMain;
						if (MarioForHold->isHoldingKoopas == false)
							MarioForHold->isHoldingKoopas = true;
						if (isHold == false)
							isHold = true;
						state = KOOPAS_STATE_ISHOLD;
					}
				}
				else
				{
					if (Mario_x > x - 24&&Mario_x<x)
					{
						MarioForHold = MarioMain;
						if (MarioForHold->isHoldingKoopas == false)
							MarioForHold->isHoldingKoopas = true;
						if (isHold == false)
							isHold = true;
						state = KOOPAS_STATE_ISHOLD;
					}
				}				
			}
			else
			{
				if (Mario_x < x + 24&&Mario_x>x)
				{
					MarioForHold = MarioMain;
					if (MarioForHold->isHoldingKoopas == false)
						MarioForHold->isHoldingKoopas = true;
					if (isHold == false)
						isHold = true;
					state = KOOPAS_STATE_ISHOLD;
				}
			}
		}
		
	}
	else
	{
		if(level!=KOOPAS_LEVEL_FLY2)
			vy += KOOPAS_GRAVITY * dt;
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		// turn off collision when die 
		if(state!=KOOPAS_STATE_KICKOUT)
			CalcPotentialCollisions(coObjects, coEvents);
		
		// reset untouchable timer if untouchable time has passed
		// No collision occured, proceed normally
		if (coEvents.size() == 0)
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

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
			/*if (nx != 0) vx = 0;*/
			if (ny != 0)
			{
				vy = 0;
				isOnGround = true;
			}
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CMario*>(e->obj))
				{
					SetPosition(x, y - 1);
					MarioForHold = MarioMain;
					if (nx != 0)
					{
						if (MarioForHold->untouchable == false)
						{
							if (state != KOOPAS_STATE_ISHOLD)
							{
								if (state == KOOPAS_STATE_DEFENSE_STATIC||state==KOOPAS_STATE_KICKOUT_2)
								{
									if (MarioForHold->GetState() == MARIO_STATE_RUNNING_RIGHT || MarioForHold->GetState() == MARIO_STATE_RUNNINGFAST_RIGHT)
									{
										if (MarioForHold->isHoldingKoopas == true)
											return;
										isHold = true;
										state = KOOPAS_STATE_ISHOLD;
										MarioForHold->isHoldingKoopas = true;
									}
									else if (MarioForHold->GetState() == MARIO_STATE_RUNNING_LEFT || MarioForHold->GetState() == MARIO_STATE_RUNNINGFAST_LEFT)
									{
										if (MarioForHold->isHoldingKoopas == true)
											return;
										isHold = true;
										state = KOOPAS_STATE_ISHOLD;
										MarioForHold->isHoldingKoopas = true;
									}
									else
									{
										state = KOOPAS_STATE_DEFENSE_DYNAMIC;
										if (MarioMain->Getnx() > 0)
											vx = KOOPAS_DYNAMIC_SPEED;
										else
											vx = -KOOPAS_DYNAMIC_SPEED;
									}
									
								}
								else
								{
									if (MarioForHold->GetLevel() > MARIO_LEVEL_SMALL)
									{
										MarioForHold->StartUntouchable();
										if (MarioMain->GetLevel() == MARIO_LEVEL_FIRE || MarioMain->GetLevel() == MARIO_LEVEL_TAIL)
											MarioMain->SetLevel(MARIO_LEVEL_BIG);
										else
											MarioMain->SetLevel(MARIO_LEVEL_SMALL);
									}
									else
										MarioForHold->SetState(MARIO_STATE_DIE);
								}
							}
							else
							{
								if (MarioForHold->isHoldingKoopas == true)
								{
									if (MarioForHold->GetLevel() != MARIO_LEVEL_SMALL)
									{
										if (MarioForHold->Getnx() < 0)
										{
											if (MarioForHold->GetLevel() == MARIO_LEVEL_TAIL)
												SetPosition(Mario_x + 20.00f, Mario_y + 7.00f);
											else
												SetPosition(Mario_x + 13.00f, Mario_y + 7.00f);
										}
										else
											SetPosition(Mario_x - 14.00f, Mario_y + 7.00f);
									}
									else
									{
										if (MarioForHold->Getnx() < 0)
											SetPosition(Mario_x + 11, Mario_y - 3);
										else
											SetPosition(Mario_x - 13, Mario_y - 3);
									}
								}
								else
								{
									state = KOOPAS_STATE_DEFENSE_DYNAMIC;
									if (MarioMain->Getnx() > 0)
										vx = KOOPAS_DYNAMIC_SPEED;
									else
										vx = -KOOPAS_DYNAMIC_SPEED;
								}
							}
						}
					}
					if (ny > 0&&nx==0)
					{
						SetPosition(x, y - 1);
						float Mario_vx, Mario_vy;
						MarioMain->GetSpeed(Mario_vx, Mario_vy);
						if (level == KOOPAS_LEVEL_FLY||level==KOOPAS_LEVEL_FLY2)
						{
							MarioMain->SetSpeed(Mario_vx, -MARIO_JUMP_DEFLECT_SPEED);
							level = KOOPAS_LEVEL_NORMAL;
							state = KOOPAS_STATE_WALKING;
							
						}						
						else
						{
							MarioMain->SetSpeed(Mario_vx, -MARIO_JUMP_DEFLECT_SPEED);
							if (state == KOOPAS_STATE_DEFENSE_STATIC||state==KOOPAS_STATE_KICKOUT_2)
							{
								CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
								state = KOOPAS_STATE_DEFENSE_DYNAMIC;
								if (MarioMain->Getnx() > 0)
									vx = KOOPAS_DYNAMIC_SPEED;
								else
									vx = -KOOPAS_DYNAMIC_SPEED;													
							}
							else
							{
								StartWait();
								CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
								state = KOOPAS_STATE_DEFENSE_STATIC;
								vx = 0;
								vy = 0;
							}
						}
						
					}
					if (ny < 0&&nx==0)
					{
						MarioMain->SetPosition(Mario_x, Mario_y - 1);
						if (MarioMain->untouchable == false)
						{
							if (state == KOOPAS_STATE_DEFENSE_DYNAMIC||state==KOOPAS_STATE_WALKING)
							{
								if (MarioMain->GetLevel() > MARIO_LEVEL_SMALL)
								{
									MarioMain->StartUntouchable();
									if (MarioMain->GetLevel() == MARIO_LEVEL_FIRE || MarioMain->GetLevel() == MARIO_LEVEL_TAIL)
										MarioMain->SetLevel(MARIO_LEVEL_BIG);
									else
										MarioMain->SetLevel(MARIO_LEVEL_SMALL);
								}
								else
									MarioMain->SetState(MARIO_STATE_DIE);
							}
						}
					}
				}
				if (dynamic_cast<CBrick*>(e->obj))
				{
					if (nx != 0)
					{
						vx = -vx;
					}						
				}
				else if (dynamic_cast<CHiddenObject*>(e->obj))
				{
					if (nx != 0)
					{

						if (state != KOOPAS_STATE_WALKING)
						{
							x += dx;
						}
						else
							vx = -vx;
					}
				}
				else if (dynamic_cast<CGoomba*>(e->obj))
				{
					if (nx != 0)
					{
						if (state == KOOPAS_STATE_DEFENSE_DYNAMIC)
						{
							x += dx;
							//y += dy;
							CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
							if (goomba->GetState() != GOOMBA_STATE_DIE)
							{
								float goomba_x, goomba_y;
								float goomba_vx, goomba_vy;
								goomba->GetPosition(goomba_x, goomba_y);
								goomba->SetPosition(goomba_x, goomba_y - 1);
								if (goomba->GetLevel() == GOOMBA_LEVEL_FLY)
									goomba->Setlevel(GOOMBA_LEVEL_NORMAL);
								goomba->SetState(GOOMBA_STATE_KICKED_OUT);

								goomba->GetSpeed(goomba_vx, goomba_vy);
								goomba->SetSpeed(goomba_vx, -0.24f);

								CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
								
							}							
						}
						else
							vx = -vx;
					}
				}
				else if (dynamic_cast<CKoopas*>(e->obj))
				{
					if (nx != 0)
					{
						if (state == KOOPAS_STATE_DEFENSE_DYNAMIC)
						{
							SetPosition(x, y - 1);
							CKoopas* koopas = dynamic_cast<CKoopas*>(e->obj);
							if (koopas->GetLevel() == KOOPAS_LEVEL_FLY || koopas->GetLevel() == KOOPAS_LEVEL_FLY2)
								koopas->Setlevel(KOOPAS_LEVEL_NORMAL);
							if (koopas->GetState() != KOOPAS_STATE_DEFENSE_DYNAMIC)
							{
								koopas->SetState(KOOPAS_STATE_KICKOUT);
								koopas->SetSpeed(0, -0.24f);
							}
						}
					}
				}
			}
		}
		
		for (UINT i = 0; i < coEvents.size(); i++)
			delete coEvents[i];
	}
	
	
}
void CKoopas::StartWait()
{
	isWaiting = true; 
	wait_start = (DWORD)GetTickCount64();
}
void CKoopas::Render()
{
	int ani = -1;
	/*if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}*/
		
	if (vx == 0&&level!=KOOPAS_LEVEL_FLY2)
		ani = KOOPAS_ANI_DEFENSE_STATIC;
	else if(vx>0)
	{
		if (state == KOOPAS_STATE_WALKING)
		{
			if (level == KOOPAS_LEVEL_FLY||level==KOOPAS_LEVEL_FLY2)
				ani = KOOPAS_ANI_FLY_WALKING_RIGHT;
			else
				ani = KOOPAS_ANI_WALKING_RIGHT;
		}			
		else
			ani = KOOPAS_ANI_DEFENSE_DYNAMIC;
	}
	else
	{
		if (state == KOOPAS_STATE_WALKING)
		{
			if (level == KOOPAS_LEVEL_FLY || level == KOOPAS_LEVEL_FLY2)
				ani = KOOPAS_ANI_FLY_WALKING_LEFT;
			else
				ani = KOOPAS_ANI_WALKING_LEFT;
		}			
		else
			ani = KOOPAS_ANI_DEFENSE_DYNAMIC;
	}
	if (state == KOOPAS_STATE_KICKOUT||state==KOOPAS_STATE_KICKOUT_2)
		ani = KOOPAS_ANI_KICKOUT;
	animation_set->at(ani)->Render(round(x), round(y));

	/*RenderBoundingBox();*/
}
void CKoopas::stop()
{
	if (abs(vx) == KOOPAS_DYNAMIC_SPEED)
	{
		vx = 0;
		vy = 0;
	}
	else
	{
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE;
		vx = 0;
		vy = 0;
	}
}
void CKoopas::left()
{
	nx = -1;
}
void CKoopas::right()
{
	nx = 1;
}
void CKoopas::walk()
{
	vx = nx*KOOPAS_WALKING_SPEED;
}
void CKoopas::dynamic()
{
	vx = nx * KOOPAS_DYNAMIC_SPEED;
}
void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	//case KOOPAS_STATE_DIE:
		//y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE -1;
		//vx = 0;
		//vy = 0;
		//break;
	case KOOPAS_STATE_WALKING:
		walk();
		break;
	case KOOPAS_STATE_DEFENSE_STATIC:
		stop();
		break;
	case KOOPAS_STATE_DEFENSE_DYNAMIC:
		dynamic();
		break;
	case KOOPAS_STATE_ISHOLD:
		stop();
		break;
	}
}
CKoopas::~CKoopas()
{

}