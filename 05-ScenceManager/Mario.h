#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_RUNNING_SPEED		0.2f
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f		//0.00095f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_RUNNING_RIGHT	101
#define MARIO_STATE_RUNNING_LEFT	201
#define MARIO_STATE_CHANGERIGHT		102
#define MARIO_STATE_CHANGELEFT		202
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400


#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5
#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7
#define MARIO_ANI_BIG_CHANGE_RIGHT			8
#define MARIO_ANI_BIG_CHANGE_LEFT			9
#define MARIO_ANI_BIG_JUMP_RIGHT			10
#define MARIO_ANI_BIG_JUMP_LEFT				11
#define MARIO_ANI_SMALL_CHANGE_RIGHT		12
#define MARIO_ANI_SMALL_CHANGE_LEFT			13
#define MARIO_ANI_SMALL_JUMP_RIGHT			14
#define MARIO_ANI_SMALL_JUMP_LEFT			15


#define MARIO_ANI_DIE				20

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
	DWORD StartMovingLeft;
	DWORD StartMovingRight;

	
public:
	bool isOnGround;

	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void Stop();
	void Walk();
	void Run();
	void Jump();
	void Right();
	void Left();
	void ChangeDirectionRight();
	void ChangeDirectionLeft();

	void SetTimeMovingRight(DWORD t) { this->StartMovingRight = t; }
	void SetTimeMovingLeft(DWORD t) { this->StartMovingLeft = t; }
	DWORD GetTimeMovingRight() { return StartMovingRight; }
	DWORD GetTimeMovingLeft() { return StartMovingLeft; }
	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void CollisionWithBrick(vector<LPGAMEOBJECT>* coObject = NULL);
	void CollisionWithBountyBrick(vector<LPGAMEOBJECT>* coObject = NULL);
	void CollisionWithEnemy(vector<LPGAMEOBJECT>* coObject = NULL);
	void CollisionWithTerrain(vector<LPGAMEOBJECT>* coObject = NULL);
	void CollisionWithBounty(vector<LPGAMEOBJECT>* coObject = NULL);
	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};