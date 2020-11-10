#pragma once
#include "GameObject.h"
#include "FireBall.h"

#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_RUNNING_SPEED		0.3f
#define MARIO_RUNNINGFAST_SPEED	0.4f
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMPHIGH_SPEED_Y	0.7f
#define MARIO_FLY_SPEED_Y		0.3f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_RESIST_GRAVITY	0.002f	//0.00095f
#define MARIO_DIE_DEFLECT_SPEED	 0.3f
#define MARIO_DX_RUNNING_LIMIT		256

#define MARIO_STATE_IDLE				0
#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_RUNNING_RIGHT		101
#define MARIO_STATE_RUNNING_LEFT		201
#define MARIO_STATE_CHANGERIGHT			102
#define MARIO_STATE_CHANGELEFT			202
#define MARIO_STATE_RUNNINGFAST_RIGHT	103
#define MARIO_STATE_RUNNINGFAST_LEFT	203
#define MARIO_STATE_ATTACK				500
#define MARIO_STATE_JUMP				300
#define MARIO_STATE_HIGHJUMP			301
#define MARIO_STATE_FLYRIGHT			302
#define MARIO_STATE_FLYLEFT				303
#define MARIO_STATE_FLYFALL				304
#define MARIO_STATE_DIE					400

	
#define MARIO_ANI_BIG_IDLE_RIGHT			0
#define MARIO_ANI_BIG_IDLE_LEFT				1
#define MARIO_ANI_SMALL_IDLE_RIGHT			2
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
#define MARIO_ANI_TAIL_IDLE_RIGHT			16
#define MARIO_ANI_TAIL_IDLE_LEFT			17
#define MARIO_ANI_TAIL_WALKING_RIGHT		18
#define MARIO_ANI_TAIL_WALKING_LEFT			19
#define MARIO_ANI_TAIL_CHANGE_RIGHT			20
#define MARIO_ANI_TAIL_CHANGE_LEFT			21
#define MARIO_ANI_TAIL_JUMP_RIGHT			22
#define MARIO_ANI_TAIL_JUMP_LEFT			23
#define MARIO_ANI_FIRE_IDLE_RIGHT			24
#define MARIO_ANI_FIRE_IDLE_LEFT			25
#define MARIO_ANI_FIRE_WALKING_RIGHT		26
#define MARIO_ANI_FIRE_WALKING_LEFT			27
#define MARIO_ANI_FIRE_CHANGE_RIGHT			28
#define MARIO_ANI_FIRE_CHANGE_LEFT			29
#define MARIO_ANI_FIRE_JUMP_RIGHT			30
#define MARIO_ANI_FIRE_JUMP_LEFT			31

#define MARIO_ANI_BIG_RUNNING_RIGHT			32
#define MARIO_ANI_BIG_RUNNING_LEFT			33
#define MARIO_ANI_TAIL_RUNNING_RIGHT		34
#define MARIO_ANI_TAIL_RUNNING_LEFT			35
#define MARIO_ANI_FIRE_RUNNING_RIGHT		36
#define MARIO_ANI_FIRE_RUNNING_LEFT			37
#define MARIO_ANI_SMALL_RUNNING_RIGHT		38
#define MARIO_ANI_SMALL_RUNNING_LEFT		39

#define MARIO_ANI_TAIL_FLYRIGHT				40
#define MARIO_ANI_TAIL_FLYLEFT				41
#define MARIO_ANI_TAIL_FALL_FLYRIGHT		42
#define MARIO_ANI_TAIL_FALL_FLYLEFT			43

#define MARIO_ANI_FIRE_ATTACK_RIGHT			44
#define MARIO_ANI_FIRE_ATTACK_LEFT			45

#define MARIO_ANI_DIE						46

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	3
#define MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_TAIL_BBOX_WIDTH	15
#define MARIO_TAIL_BBOX_HEIGHT	28

#define MARIO_FIRE_BBOX_WIDTH	15
#define MARIO_FIRE_BBOX_HEIGHT	27

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 

	CFireBall* FireBall;

	DWORD StartMovingLeft;
	DWORD StartMovingRight;

	DWORD StartRunningLeft;
	DWORD StartRunningRight;

	DWORD StartRunningLeftFast;
	DWORD StartRunningRightFast;

	DWORD StartFly;

	
public:
	bool isOnGround;
	//bool allowJump;
	bool isRunningRight;
	bool isRunningLeft;

	bool isRunningFastRight;
	bool isRunningFastLeft;

	bool isFlying;						//for identifying sprites when flying and calculate fly time
	bool isFlyFall;						//for choosing the right sprites for jump and fall fly state	

	bool isAttacking;

	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();
	void Stop();
	void Walk();
	void Run();	
	void Jump();
	//void JumpHigh();
	void Right();
	void Left();
	void RunFast();
	void Fly();
	void Fall();
	void Attack();
	void ChangeDirectionRight();
	void ChangeDirectionLeft();

	void SetTimeMovingRight(DWORD t) { this->StartMovingRight = t; }
	void SetTimeMovingLeft(DWORD t) { this->StartMovingLeft = t; }
	DWORD GetTimeMovingRight() { return StartMovingRight; }
	DWORD GetTimeMovingLeft() { return StartMovingLeft; }

	void SetTimeRunningRight(DWORD t) { this->StartRunningRight = t; }
	void SetTimeRunningLeft(DWORD t) { this->StartRunningLeft = t; }
	DWORD GetTimeRuningRight() { return StartRunningRight; }
	DWORD GetTimeRunningLeft() { return StartRunningLeft; }

	void SetTimeRunningFastRight(DWORD t) { this->StartRunningRightFast = t; }
	void SetTimeRunningFastLeft(DWORD t) { this->StartRunningLeftFast = t; }
	DWORD GetTimeRunningFastRight() { return StartRunningRightFast; }
	DWORD GetTimeRunningFastLeft() { return StartRunningLeftFast; }

	void SetTimeFly(DWORD t) { this->StartFly = t; }
	DWORD GetTimeFly() { return StartFly; }

	CFireBall* GetFireBall() { return this->FireBall; }

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();
	void GetBoundingBoxTailLevel(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};