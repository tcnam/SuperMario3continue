#include"Bounty.h"
CBounty::CBounty()
{
	start_x = start_y = 0;
	Mario = NULL;
	isRightDirection = false;
	isUsed = false;
	isLeaf = false;
	isCrossBoundary = false;
	isFinised = false;
}
void CBounty::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state != BOUNTY_STATE_COIN)
	{
		right = x + BOUNTY_BBOX_WIDTH;
		bottom = y + BOUNTY_BBOX_HEIGHT;
	}
	else
	{
		right = x + BOUNTY_COIN_BBOX_WIDTH;
		bottom = y + BOUNTY_COIN_BBOX_HEIGHT;
	}	
}

void CBounty::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	
	if (isUsed == false)
		return;
	if (isFinised == true)
	{
		SetPosition(start_x, start_y);
		isUsed = false;
	}
	CGameObject::Update(dt, coObjects);
	if (state == BOUNTY_STATE_POWERUP)				//powerup include red mushroom and super leaf
	{
		if (isLeaf == false)						//red mushroom 
		{
			if (start_y - BOUNTY_MUSHROOM_DISTANCE_DY_TOHAVE_VX < y)
			{
				if(isCrossBoundary==false)
					vy = -BOUNTY_MUSHROOM_VY;
				else
				{
					if (isRightDirection == true)
						vx = BOUNTY_MUSHROOM_VX;
					else
						vx = -BOUNTY_MUSHROOM_VX;
					vy += BOUNTY_GRAVITY * dt;
				}
			}
			else
			{
				isCrossBoundary = true;
				if (isRightDirection == true)
					vx = BOUNTY_MUSHROOM_VX;
				else
					vx = -BOUNTY_MUSHROOM_VX;
				vy += BOUNTY_GRAVITY * dt;
			}						
		}
		else//super leaf
		{
			if (start_y - BOUNTY_LEAF_DISTANCE_DY_TOFALL < y)
			{
				if (isCrossBoundary == false)
					vy = -BOUNTY_LEAF_VY;
				else
				{
					if (isCrossBoundary == false)
					{
							vx = BOUNTY_LEAF_VX;
					}
					vy = BOUNTY_GRAVITY;
					if (start_x +  BOUNTY_LEAF_DISTANCE_DX_TOCHANGE_VX <= x)
						vx = -BOUNTY_LEAF_VX;
					else if (start_x >= x)
						vx = BOUNTY_LEAF_VX;
				}					
			}
			else
			{
				if (isCrossBoundary == false)
				{
						vx = BOUNTY_LEAF_VX;
				}					
				isCrossBoundary = true;
				vy = BOUNTY_GRAVITY;
				if (start_x + BOUNTY_LEAF_DISTANCE_DX_TOCHANGE_VX <= x)
					vx = -BOUNTY_LEAF_VX;
				else if (start_x>= x)
					vx = BOUNTY_LEAF_VX;
			}			
		}		
	}
	else if (state == BOUNTY_STATE_LIFEUP)			//green mushroom 
	{
		if (start_y - BOUNTY_MUSHROOM_DISTANCE_DY_TOHAVE_VX < y)
		{
			if (isCrossBoundary == false)
				vy = -BOUNTY_MUSHROOM_VY;
			else
			{
				if (isRightDirection == true)
					vx = BOUNTY_MUSHROOM_VX;
				else
					vx = -BOUNTY_MUSHROOM_VX;
				vy += BOUNTY_GRAVITY * dt;
			}
				
		}
		else
		{
			isCrossBoundary = true;
			if (isRightDirection == true)
				vx = BOUNTY_MUSHROOM_VX;
			else
				vx = -BOUNTY_MUSHROOM_VX;
			vy += BOUNTY_GRAVITY * dt;
		}
	}
	else//coin inside bounty brick
	{
		if (start_y - BOUNTY_COIN_DISTANCE_DY_TOFALL < y)
		{			
			if (isCrossBoundary == false)
				vy = -BOUNTY_COIN_VY;
			else
				vy += BOUNTY_GRAVITY * dt;
		}
		else
		{
			isCrossBoundary = true;
			vy += BOUNTY_GRAVITY * dt;
		}
	}	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	float Mario_current_x, Mario_current_y;
	Mario->GetPosition(Mario_current_x, Mario_current_y);
	if (state != BOUNTY_STATE_COIN)
	{
		if (AABBCheck(Mario) == true)//check if bounty super leaf collide with mario in this frame
		{
			isFinised = true;
			Mario->SetPosition(Mario_current_x, Mario_current_y - 1);//push mario a distance to avoid falling
			/*SetPosition(start_x, start_y + 320.00f);*/
			if (state == BOUNTY_STATE_POWERUP)
			{
				CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 1000);
				if (Mario->GetLevel() < MARIO_LEVEL_BIG)
					Mario->SetLevel(MARIO_LEVEL_BIG);
				else
					Mario->SetLevel(MARIO_LEVEL_TAIL);
			}
			
			
		}
		if (isLeaf == true && isCrossBoundary == true&&state==BOUNTY_STATE_POWERUP)
		{
			x += dx;
			y += dy;
		}
	}
	//turn off collision of bounty(state leaf) when it get over its limit boundary and begin to fall
		
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
		y += min_ty * dy + ny * 0.4f;
		x += min_tx * dx + nx * 0.4f;
		if (ny < 0)
		{
			vy = 0;
			if (state == BOUNTY_STATE_COIN)
			{
				CGame::GetInstance()->SetCoins(CGame::GetInstance()->GetCoins() + 1);
				CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 100);
				isFinised = true;
			}				
		}
		if(nx!=0&&isLeaf==false)
			vx = -vx;
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario*>(e->obj))
			{				
				vy = BOUNTY_GRAVITY;
				x += dx;
				y += dy;
				if (state != BOUNTY_STATE_COIN)
				{
					/*SetPosition(start_x, start_y+320.00f);*/
					isFinised = true;
					if (Mario->GetLevel() == MARIO_LEVEL_SMALL)
						Mario->SetPosition(Mario_current_x, Mario_current_y - MARIO_BIG_BBOX_HEIGHT + MARIO_SMALL_BBOX_HEIGHT - 1);
					else
						Mario->SetPosition(Mario_current_x, Mario_current_y - 1);
					if (state == BOUNTY_STATE_POWERUP)
					{
						CGame::GetInstance()->SetScores(CGame::GetInstance()->GetScores() + 1000);
						if (isLeaf == true)
						{
							Mario->GetEffect()->SetState(EFFECT_CLOUND);
							Mario->GetEffect()->SetPosition(Mario_current_x, Mario_current_y);
							Mario->StartTransForm();
							Mario->SetLevel(MARIO_LEVEL_TAIL);
						}							
						else
						{
							Mario->GetEffect()->SetState(EFFECT_SMALL_BIG);
							if (Mario->nx >= 0)
								Mario->GetEffect()->RightOrLeft = true;
							else
								Mario->GetEffect()->RightOrLeft = false;
							Mario->GetEffect()->SetPosition(Mario_current_x, Mario_current_y+MARIO_SMALL_BBOX_HEIGHT-MARIO_BIG_BBOX_HEIGHT);
							Mario->StartTransForm();
							Mario->SetLevel(MARIO_LEVEL_BIG);
						}
							
					}					
				}
			}
			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (isLeaf == true)
				{

					vy = BOUNTY_GRAVITY;
					x += dx;
					y += dy;
				}
				
			}
		}
	}
	
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];	
}

void CBounty::Render()
{
	if (isUsed == false)
		return;
	if (isFinised == true)
		return;
	int ani = -1;
	if (state == BOUNTY_STATE_POWERUP)
	{
		if (isLeaf == true)
			ani = BOUNTY_LEAF_ANI;
		else
			ani = BOUNTY_RED_MUSHROOM_ANI;
	}
	else if (state == BOUNTY_STATE_LIFEUP)
	{
		ani = BOUNTY_GREEN_MUSHROOM_ANI;
	}
	else
		ani = BOUNTY_COIN_ANI;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CBounty::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOUNTY_STATE_COIN:
		vy = 0;
		vx = 0;
		break;
	case BOUNTY_STATE_POWERUP:
		vy = 0;
		vx = 0;
		break;
	case BOUNTY_STATE_LIFEUP:
		vx = 0;
		vy = 0;
		break;

	}
}
CBounty::~CBounty()
{

}