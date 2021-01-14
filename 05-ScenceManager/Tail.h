#pragma once
#pragma once
#include"GameObject.h"

#define TAIL_SPEED		0.05f

#define TAIL_DX_MOVE	8.0f

#define TAIL_BBOX_WIDTH		8
#define TAIL_BBOX_HEIGHT	8
class CTail :public CGameObject
{
public:
	bool isUsed;
	CTail();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	~CTail();
};