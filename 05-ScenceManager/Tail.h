#pragma once
#pragma once
#include"GameObject.h"

#define TAIL_SPEED		0.05f

#define TAIL_DX_MOVE	24.0f

#define TAIL_BBOX_WIDTH		8
#define TAIL_BBOX_HEIGHT	8

class CTail :public CGameObject
{
	float start_x;
	float start_y;
public:
	bool isUsed;
	CTail();
	void SetStartPosition(float lo_x, float lo_y) { start_x = lo_x; start_y = lo_y; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	~CTail();
};