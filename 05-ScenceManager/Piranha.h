#include "GameObject.h"
#include "Mario.h"

#define FLOWER_FLY_SPEED 0.015f;	

#define FLOWER_BBOX_WIDTH 16
#define FLOWER_BBOX_HEIGHT 24

#define FLOWER_UPPER_Y			-72
#define FLOWER_LOWER_Y			-40
#define FLOWER_APPEAR_Y			-48

#define FLOWER_TIMEWAIT	1000
class CPiranha : public CGameObject
{
	float start_x;
	float start_y;
	//CEffect* effect;
	CMario* Mario;
public:	
	bool isShooting;
	bool isFinish;
	DWORD Shoot_Start;
	DWORD Finish_Start;
	CPiranha();
	void SetInitPosition(float x, float y) { start_x = x; start_y = y; }
	float GetStartx() { return start_x; }
	float GetStarty() { return start_y; }
	void StartShoot() { isShooting = true; Shoot_Start = (DWORD)GetTickCount64(); vy = 0; }
	void StartFinish() { isFinish = true; Finish_Start = (DWORD)GetTickCount64(); }
	void SetMario(CMario* mario) { Mario = mario; }
	/*void SetEffect(CEffect* ef) { effect = ef; }
	CEffect* GetEffect() { return effect; }*/
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	~CPiranha();
};
