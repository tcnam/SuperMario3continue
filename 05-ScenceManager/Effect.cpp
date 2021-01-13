#pragma once
#include "Effect.h"


CEffect::CEffect()
{
	x = 0;
	y = 0;
	animation_set = NULL;	
	state = 0;
	dt = 0;
	owner_id = 0;
	RightOrLeft = false;
}
void CEffect::Render()
{
	if(owner_id==EFFECT_OWNER_ID_FIREFLOWER)
		animation_set->at(0)->Render(round(x), round(y));
	else
	{
		if (state == EFFECT_SMALL_BIG)
		{
			if(RightOrLeft==true)
				animation_set->at(1)->Render(round(x), round(y));
			else
				animation_set->at(2)->Render(round(x), round(y));
		}
		else
			animation_set->at(0)->Render(round(x), round(y));
		
			
	}
	
}
void CEffect::Update(DWORD dt)
{
	this->dt = dt;
}
CEffect::~CEffect()
{

}