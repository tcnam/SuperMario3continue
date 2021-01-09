#include "Effect.h"
#pragma once

CEffect::CEffect()
{
	x = 0;
	y = 0;
	sprite = NULL;
}
void CEffect::Draw(float x, float y, int alpha)
{
	CGame* game = CGame::GetInstance();
	LPSPRITE sprite = CEffect::GetSprite();
	game->Draw(x, y, sprite->GetText(), sprite->GetLeft(), sprite->GetTop(), sprite->GetRight(), sprite->GetBottom(), alpha);
}
CEffect::~CEffect()
{

}