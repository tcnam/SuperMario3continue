#pragma once

#include "GameObject.h"
#include "Mario.h"
#include "Game.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height; 
	CMario* Mario;
public:
	CPortal(float l, float t, float r, float b, int scene_id);
	void SetMario(CMario* mario) { Mario = mario; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	int GetSceneId() { return scene_id;  }
	~CPortal();
};