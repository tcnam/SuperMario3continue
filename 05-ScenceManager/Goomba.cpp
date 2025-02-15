#include "Goomba.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
	level = 0;
	init_level = 0;
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	if (level == GOOMBA_LEVEL_FLY)
	{
		if (vy<0)
		{
			right = x + GOOMBA_FLY_JUMP_BBOX_WIDTH;
			bottom = y + GOOMBA_FLY_JUMP_BBOX_HEIGHT;
		}
		else
		{
			right = x + GOOMBA_FLY_BBOX_WIDTH;
			bottom = y + GOOMBA_FLY_BBOX_HEIGHT;
		}
		
	}
	else
	{
		right = x + GOOMBA_BBOX_WIDTH;
		if (state == GOOMBA_STATE_DIE)
			bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
		else
			bottom = y + GOOMBA_BBOX_HEIGHT;
	}
	
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	if (Mario == NULL)
		return;
	if (Mario->GetState() == MARIO_STATE_DIE)
		return;
	if (Mario->isTransform == true)
		return;
	float Mario_x, Mario_y;
	Mario->GetPosition(Mario_x, Mario_y);
	if (GetTickCount64() - untouchable_start > GOOMBA_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = false;
		//SetPosition(x, y + 320.00f);
		if (level == GOOMBA_LEVEL_FLY)
		{
			if (abs(x - start_x) >= GOOMBA_DX_LIMIT_TOFLY)
			{
				SetPosition(x, y - GOOMBA_FLY_JUMP_BBOX_WIDTH);
				vy = -GOOMBA_FLY_JUMP_SPEED;
				//state = GOOMBA_STATE_JUMP;
				if (Mario_x < x)
					vx = -GOOMBA_WALKING_SPEED;
				start_x = x;
			}
			else
				state = GOOMBA_STATE_WALKING;
		}
		CGameObject::Update(dt, coObjects);
		vy += GOOMBA_GRAVITY * dt;
		if (state != GOOMBA_STATE_DIE)
		{
			if (AABBCheck(Mario) == true)
			{
				if (Mario->isAttacking == true||Mario->isHoldingKoopas==true)
				{
					if (level == GOOMBA_LEVEL_FLY)
					{
						level = GOOMBA_LEVEL_NORMAL;
						state = GOOMBA_STATE_WALKING;
						CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
					}
					else
					{
						if (state != GOOMBA_STATE_DIE)
						{
							state = GOOMBA_STATE_DIE;
							SetPosition(x, y - GOOMBA_BBOX_HEIGHT_DIE + GOOMBA_BBOX_HEIGHT);
							CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
							vx = 0;
							vy = 0;
						}
					}				
				}
				if (Mario->untouchable == false)
				{
					if (/*state == GOOMBA_STATE_JUMP || */state == GOOMBA_STATE_WALKING)
					{
						SetPosition(x, y - 1);
						if (Mario->GetLevel() == MARIO_LEVEL_SMALL)
							Mario->SetState(MARIO_STATE_DIE);
						else
						{
							Mario->StartUntouchable();
							if (Mario->GetLevel() == MARIO_LEVEL_FIRE || Mario->GetLevel() == MARIO_LEVEL_TAIL)
								Mario->SetLevel(MARIO_LEVEL_SMALL);
							else
								Mario->SetLevel(MARIO_LEVEL_SMALL);
							if (Mario->GetEffect() != NULL)
							{
								Mario->GetEffect()->SetState(EFFECT_CLOUND);
								Mario->GetEffect()->SetPosition(Mario_x, Mario_y);
								Mario->StartTransForm();
							}
						}
					}
				}
				else 
				{

				}
			}
		}
		

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;
		coEvents.clear();
		if(state==GOOMBA_STATE_WALKING/*||state==GOOMBA_STATE_JUMP*/)
			CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny = 0;
			float rdx = 0;
			float rdy = 0;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
			/*if (nx != 0) vx=-vx;*/
			if (ny != 0) vy = 0;
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<CBrick*>(e->obj))
				{
					if (nx != 0)
						vx = -vx;
				}
				else if (dynamic_cast<CBountyBrick*>(e->obj))
				{
					if (nx != 0)
						vx = -vx;
				}
				else if (dynamic_cast<CGoomba*>(e->obj))
				{
					SetPosition(x, y - 1);
					if (nx != 0)
					{
						
						CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
						goomba->SetSpeed(vx, 0);
						vx = -vx;
					}
						
				}
				else if (dynamic_cast<CMario*>(e->obj))
				{
					SetPosition(x, y - 1);
					if (untouchable == false)
					{
						if (nx != 0&&ny==0)
						{						
							if (Mario->untouchable == false)
							{
								if (Mario->GetLevel() == MARIO_LEVEL_SMALL)
									Mario->SetState(MARIO_STATE_DIE);
								else
								{
									Mario->StartUntouchable();
									if (Mario->GetLevel() == MARIO_LEVEL_FIRE || Mario->GetLevel() == MARIO_LEVEL_TAIL)
										Mario->SetLevel(MARIO_LEVEL_BIG);
									else
										Mario->SetLevel(MARIO_LEVEL_SMALL);
									if (Mario->GetEffect() != NULL)
									{
										Mario->GetEffect()->SetState(EFFECT_CLOUND);
										Mario->GetEffect()->SetPosition(Mario_x, Mario_y);
										Mario->StartTransForm();
									}
								}
							}
						}
						if (ny > 0&&nx==0)
						{

							float Mario_vx, Mario_vy;
							Mario->GetSpeed(Mario_vx, Mario_vy);
							Mario->SetSpeed(Mario_vx, -MARIO_JUMP_DEFLECT_SPEED);
							if (level == GOOMBA_LEVEL_FLY)
							{
								level = GOOMBA_LEVEL_NORMAL;
								state = GOOMBA_STATE_WALKING;
								CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
							}
							else
							{
								if (state != GOOMBA_STATE_DIE)
								{
									StartUntouchable();
									state = GOOMBA_STATE_DIE;
									CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
									SetPosition(x, y - GOOMBA_BBOX_HEIGHT_DIE + GOOMBA_BBOX_HEIGHT);
									vx = 0;
									vy = 0;
								}
							}
						}
						if (ny < 0 && nx == 0)
						{
							Mario->SetPosition(Mario_x, Mario_y - 1);
							if (Mario->untouchable == false)
							{
								if (Mario->GetLevel() == MARIO_LEVEL_SMALL)
									Mario->SetState(MARIO_STATE_DIE);
								else
								{
									Mario->StartUntouchable();
									if (Mario->GetLevel() == MARIO_LEVEL_FIRE || Mario->GetLevel() == MARIO_LEVEL_TAIL)
										Mario->SetLevel(MARIO_LEVEL_BIG);
									else
										Mario->SetLevel(MARIO_LEVEL_SMALL);
								}
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

void CGoomba::Render()
{
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}
	else if (state == GOOMBA_STATE_KICKED_OUT)
	{
		ani = GOOMBA_ANI_KICKOUT;
	}
	if (level == GOOMBA_LEVEL_FLY)
	{
		if (vy<0)
			ani = GOOMBA_FLY_ANI_JUMP;
		else
			ani = GOOMBA_FLY_ANI_WALKING;
	}
	animation_set->at(ani)->Render(round(x),round(y));
	/*RenderBoundingBox();*/
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_KICKED_OUT:
		{
			vx = 0;
			vy = 0;
		}
			break;
		case GOOMBA_STATE_DIE:
			{				
				//y += GOOMBA_BBOX_HEIGHT_DIE -GOOMBA_BBOX_HEIGHT   ;
				vx = 0;
				vy = 0;
			}
				break;
		case GOOMBA_STATE_WALKING:
			vx = -GOOMBA_WALKING_SPEED;
			break;
		/*case GOOMBA_STATE_JUMP:
		{
			
			SetPosition(x, y - GOOMBA_FLY_JUMP_BBOX_WIDTH);
			
		}*/
			
			break;
	}
}
CGoomba::~CGoomba()
{

}
