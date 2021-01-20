#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_BOUNTYBRICK	4
#define OBJECT_TYPE_HIDDENOBJECT	5
#define OBJECT_TYPE_FIREBALL		6
#define OBJECT_TYPE_FIREFLOWER		7
#define OBJECT_TYPE_FLOWER_FIREBALL	8
#define OBJECT_TYPE_BOUNTY			9
#define OBJECT_TYPE_COIN			10
#define OBJECT_TYPE_FRAGMENT		11
#define OBJECT_TYPE_WEAKBRICK		12
#define OBJECT_TYPE_BOUNTYBUTTON	13
#define OBJECT_TYPE_SPECIALBRICK	14
#define OBJECT_TYPE_MYSTERYPIECE	15
#define OBJECT_TYPE_MESSAGE			16
#define OBJECT_TYPE_HIDDENDOOR		17
#define OBJECT_SYPE_AUTODOOR		18
#define OBJECT_TYPE_TAIL			19
#define OBJECT_TYPE_INTRONUMBER		20
#define OBJECT_TYPE_POINTER			21
#define OBJECT_TYPE_PIRANHA			22
#define OBJECT_TYPE_BOOMERANGBRO	23
#define OBJECT_TYPE_CURTAIN			24
#define OBJECT_TYPE_FLYBRICK		25
#define OBJECT_TYPE_BOOMERANG		26

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	int type;

	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	float ax;		

	int nx;
	//int ny;

	int state;

	DWORD dt; 

	LPANIMATION_SET animation_set;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	void SetType(int type) { this->type = type; }
	int GetType() { return this->type; }
	int GetState() { return this->state; }
	void SetDirectionnx(int nx) { this->nx = nx; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	bool AABBCheck(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }


	~CGameObject();
};

