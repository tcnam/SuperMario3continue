#pragma once
#include "Scence.h"
#include"Mario.h"
#include"Brick.h"
#include"Scence.h"

class CSceneIntro :public CScene
{
protected:
	CMario* player;
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TERRAIN(string line);
};
