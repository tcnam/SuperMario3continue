#include "Koopas.h"

CKoopas::CKoopas()
{
	vy = 0;
	right();
	SetState(KOOPAS_STATE_WALKING);

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
	if (level == KOOPAS_LEVEL_FLY)
	{
		if (abs(x - start_x) >= KOOPAS_DX_LIMIT_TOFLY)
		{
			vy = -KOOPAS_FLY_JUMP_SPEED;
			start_x = x;
		}
	}	
	CGameObject::Update(dt, coObjects);
	float Mario_x, Mario_y;
	MarioMain->GetPosition(Mario_x, Mario_y);

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

	else
	{
		vy += KOOPAS_GRAVITY * dt;
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		// turn off collision when die 
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
			}
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CMario*>(e->obj))
				{
					MarioForHold = MarioMain;
					if (nx != 0&&ny==0)
					{
						if (MarioForHold->untouchable == false)
						{
							if (state != KOOPAS_STATE_ISHOLD)
							{
								if (state != KOOPAS_STATE_DEFENSE_STATIC)
								{
									if (MarioForHold->GetLevel() > MARIO_LEVEL_SMALL)
									{
										MarioForHold->StartUntouchable();
										MarioForHold->SetLevel(MarioForHold->GetLevel() - 1);
									}
									else
										MarioForHold->SetState(MARIO_STATE_DIE);
								}
								else
								{
									if (MarioForHold->GetState() == MARIO_STATE_RUNNING_RIGHT|| MarioForHold->GetState() == MARIO_STATE_RUNNINGFAST_RIGHT)
									{
										if (MarioForHold->isHoldingKoopas == true)
											return;
										isHold = true;
										state = KOOPAS_STATE_ISHOLD;
										MarioForHold->isHoldingKoopas = true;
									}
									else if (MarioForHold->GetState() == MARIO_STATE_RUNNING_LEFT|| MarioForHold->GetState() == MARIO_STATE_RUNNINGFAST_LEFT)
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
					if (ny > 0)
					{
						SetPosition(x, y - 1);
						float Mario_vx, Mario_vy;
						MarioMain->GetSpeed(Mario_vx, Mario_vy);
						if (level == KOOPAS_LEVEL_FLY)
						{
							MarioMain->SetSpeed(Mario_vx, -MARIO_JUMP_DEFLECT_SPEED);
							level = KOOPAS_LEVEL_NORMAL;
							state = KOOPAS_STATE_WALKING;
							
						}						
						else
						{
							MarioMain->SetSpeed(Mario_vx, -MARIO_JUMP_DEFLECT_SPEED);
							if (state != KOOPAS_STATE_DEFENSE_STATIC)
							{
								CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
								state = KOOPAS_STATE_DEFENSE_STATIC;
								vx = 0;
								vy = 0;								
							}
							else
							{
								CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
								state = KOOPAS_STATE_DEFENSE_DYNAMIC;
								if (MarioMain->Getnx() > 0)
									vx = KOOPAS_DYNAMIC_SPEED;
								else
									vx = -KOOPAS_DYNAMIC_SPEED;
							}
						}
						
					}
					if (ny < 0)
					{
						if (MarioMain->untouchable == false)
						{
							if (state != KOOPAS_STATE_DEFENSE_STATIC)
							{
								if (MarioMain->GetLevel() > MARIO_LEVEL_SMALL)
								{
									MarioMain->StartUntouchable();
									MarioMain->SetLevel(MarioMain->GetLevel() - 1);
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
						if (state != KOOPAS_STATE_WALKING)
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

								goomba->SetState(GOOMBA_STATE_DIE);

								goomba->GetSpeed(goomba_vx, goomba_vy);
								goomba->SetSpeed(goomba_vx, -0.4f);

								CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
								
							}							
						}
						else
							vx = -vx;
					}
				}
			}
		}
		
		for (UINT i = 0; i < coEvents.size(); i++)
			delete coEvents[i];
	}
	
	
}

void CKoopas::Render()
{
	int ani = -1;
	/*if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_DIE;
	}*/
		
	if (vx == 0)
		ani = KOOPAS_ANI_DEFENSE_STATIC;
	else if(vx>0)
	{
		if (state == KOOPAS_STATE_WALKING)
		{
			if (level == KOOPAS_LEVEL_FLY)
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
			if (level == KOOPAS_LEVEL_FLY)
				ani = KOOPAS_ANI_FLY_WALKING_LEFT;
			else
				ani = KOOPAS_ANI_WALKING_LEFT;
		}			
		else
			ani = KOOPAS_ANI_DEFENSE_DYNAMIC;
	}
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