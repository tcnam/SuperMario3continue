#include "Terrain.h"
#include"Game.h"
#include"Utils.h"

CTerrain::CTerrain()
{
	x = 0;
	y = 0;
	sprite = NULL;
}
void CTerrain::Draw(float x, float y,int alpha)
{
	CGame* game = CGame::GetInstance();
	game->Draw(x, y, sprite->GetText(),sprite->GetLeft(),sprite->GetTop(),sprite->GetRight(),sprite->GetBottom(),alpha);
}
CTerrain::~CTerrain()
{

}