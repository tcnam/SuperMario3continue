#pragma once
#include"Font.h"
CFont::CFont()
{
	x = y = 0.0f;
	relative_x = relative_y = 0.0f;
	animation_set = NULL;
	type = 0;
	state = 0;
}
CFont::~CFont()
{

}
void CFont::Update(float ScoreBoard_x, float ScoreBoard_y)
{
	x=relative_x + ScoreBoard_x;
	y=relative_y + ScoreBoard_y;
}
void CFont::Render()
{	
	if (type == FONT_TYPE_NUMBER)
	{
		int ani = -1;
		switch(state)
		{
		case FONT_NUMBER_0:
			ani = FONT_NUMBER_0;
			break;
		case FONT_NUMBER_1:
			ani = FONT_NUMBER_1;
			break;
		case FONT_NUMBER_2:
			ani = FONT_NUMBER_2;
			break;
		case FONT_NUMBER_3:
			ani = FONT_NUMBER_3;
			break;
		case FONT_NUMBER_4:
			ani = FONT_NUMBER_4;
			break;
		case FONT_NUMBER_5:
			ani = FONT_NUMBER_5;
			break;
		case FONT_NUMBER_6:
			ani = FONT_NUMBER_6;
			break;
		case FONT_NUMBER_7:
			ani = FONT_NUMBER_7;
			break;
		case FONT_NUMBER_8:
			ani = FONT_NUMBER_8;
			break;
		case FONT_NUMBER_9:
			ani = FONT_NUMBER_9;
			break;
		}
		animation_set->at(ani)->Render(round(x), round(y));
	}
	else if(type==FONT_TYPE_STATE)
	{
		switch(state)
		{
		case FONT_STATE_WHITE:
			animation_set->at(1)->Render(round(x), round(y));
			break;
		default:
			animation_set->at(0)->Render(round(x), round(y));
			break;
		}
	}
	else if (type == FONT_TYPE_STATE_MAX)
	{
		switch (state)
		{
		case FONT_STATE_MAX_WHITE:
			animation_set->at(1)->Render(round(x), round(y));
			break;
		default:
			animation_set->at(0)->Render(round(x), round(y));
			break;
		}
	}
	else
		animation_set->at(0)->Render(round(x), round(y));

}