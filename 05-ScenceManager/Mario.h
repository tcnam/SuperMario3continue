#pragma once
#include "GameObject.h"
#include "HiddenObject.h"
#include "Brick.h"
#include "FireBall.h"
#include "Effect.h"
#include "Tail.h"
#include "FlyBrick.h"

#define MARIO_AUTO_WALKING_SPEED	0.05f
#define MARIO_WALKING_SPEED		0.1f 
#define MARIO_RUNNING_SPEED		0.15f
#define MARIO_RUNNINGFAST_SPEED	0.22f
//0.1f
#define MARIO_JUMP_SPEED_Y		0.24f				//0.33f
#define MARIO_JUMPHIGH_SPEED_Y	0.26f
#define MARIO_JUMP_MAXPOWER_SPEED_Y		0.5f
#define MARIO_FLY_SPEED_Y		0.13f
#define MARIO_JUMP_DEFLECT_SPEED 0.23f
#define MARIO_GRAVITY			0.0008f
#define MARIO_RESIST_GRAVITY	0.04f	//0.00095f
#define MARIO_DIE_DEFLECT_SPEED	 0.3f
#define MARIO_DX_RUNNING_LIMIT		256

#define MARIO_STATE_IDLE				0

#define MARIO_STATE_MOVE_UP				601
#define MARIO_STATE_MOVE_DOWN			602
#define MARIO_STATE_GOTO_ANOTHER_SCENCE	603

#define MARIO_STATE_WALKING_RIGHT		100
#define MARIO_STATE_RUNNING_RIGHT		101
#define MARIO_STATE_CHANGERIGHT			102
#define MARIO_STATE_RUNNINGFAST_RIGHT	103
#define MARIO_STATE_SLIDE_RIGHT			104

#define MARIO_STATE_WALKING_LEFT		200
#define MARIO_STATE_RUNNING_LEFT		201
#define MARIO_STATE_CHANGELEFT			202
#define MARIO_STATE_RUNNINGFAST_LEFT	203
#define MARIO_STATE_SLIDE_LEFT			204

#define MARIO_STATE_ATTACK				500
#define MARIO_STATE_DUCK				501

#define MARIO_STATE_JUMP				300
#define MARIO_STATE_HIGHJUMP			301
#define MARIO_STATE_FLYRIGHT			302
#define MARIO_STATE_FLYLEFT				303
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

#define MARIO_ANI_TAIL_ATTACK_RIGHT			44
#define MARIO_ANI_TAIL_ATTACK_LEFT			45
#define MARIO_ANI_FIRE_ATTACK_RIGHT			46
#define MARIO_ANI_FIRE_ATTACK_LEFT			47

#define MARIO_ANI_BIG_HOLDKOOPAS_IDLE_RIGHT		48
#define MARIO_ANI_BIG_HOLDKOOPAS_WALK_RIGHT		49
#define MARIO_ANI_BIG_HOLDKOOPAS_IDLE_LEFT		50
#define MARIO_ANI_BIG_HOLDKOOPAS_WALK_LEFT		51

#define MARIO_ANI_SMALL_HOLDKOOPAS_IDLE_RIGHT		52
#define MARIO_ANI_SMALL_HOLDKOOPAS_WALK_RIGHT		53
#define MARIO_ANI_SMALL_HOLDKOOPAS_IDLE_LEFT		54
#define MARIO_ANI_SMALL_HOLDKOOPAS_WALK_LEFT		55

#define MARIO_ANI_TAIL_HOLDKOOPAS_IDLE_RIGHT		56
#define MARIO_ANI_TAIL_HOLDKOOPAS_WALK_RIGHT		57
#define MARIO_ANI_TAIL_HOLDKOOPAS_IDLE_LEFT			58
#define MARIO_ANI_TAIL_HOLDKOOPAS_WALK_LEFT			59

#define MARIO_ANI_CROSS_PIPE						60

#define MARIO_ANI_BIG_DUCKING_RIGHT						61
#define MARIO_ANI_BIG_DUCKING_LEFT						62

#define MARIO_ANI_TAIL_DUCKING_RIGHT					63
#define MARIO_ANI_TAIL_DUCKING_LEFT						64

#define MARIO_ANI_FIRE_DUCKING_RIGHT					65
#define MARIO_ANI_FIRE_DUCKING_LEFT						66

#define MARIO_ANI_FIRE_HOLDKOOPAS_IDLE_RIGHT		67
#define MARIO_ANI_FIRE_HOLDKOOPAS_WALK_RIGHT		68
#define MARIO_ANI_FIRE_HOLDKOOPAS_IDLE_LEFT			69
#define MARIO_ANI_FIRE_HOLDKOOPAS_WALK_LEFT			70

#define MARIO_ANI_DIE						71

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	3
#define MARIO_LEVEL_FIRE	4

#define MARIO_BIG_BBOX_WIDTH  16
#define MARIO_BIG_BBOX_HEIGHT 27
#define MARIO_BIG_DUCK_BBOX_WIDTH	14
#define MARIO_BIG_DUCK_BBOX_HEIGHT	18

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_TAIL_BBOX_WIDTH	16
#define MARIO_TAIL_BBOX_HEIGHT	28
#define MARIO_TAIL_ATTACK_BBOX_WIDTH	24
#define MARIO_TAIL_DUCK_BBOX_WIDTH	22
#define MARIO_TAIL_DUCK_BBOX_HEIGHT	18

#define MARIO_FIRE_BBOX_WIDTH	16
#define MARIO_FIRE_BBOX_HEIGHT	27
#define MARIO_FIRE_DUCK_BBOX_WIDTH	14
#define MARIO_FIRE_DUCK_BBOX_HEIGHT	18

#define MARIO_WORLDSCENCE_BBOX_WIDTH	16
#define MARIO_WORLDSCENCE_BBOX_HEIGHT	16

#define MARIO_UNTOUCHABLE_TIME 5000
#define MARIO_FLY_TIME			4000
#define MARIO_ATTACK_TIME_TAIL	400
#define MARIO_ATTACK_TIME_FIRE	250
#define MARIO_FLYFALL_TIME	200
#define MARIO_TIME_CHANGE_DIRECTION	200
#define MARIO_RUNTIME		2000
#define MARIO_TRANSFORM_TIME	800

class CMario : public CGameObject
{
	int level;
	
	float start_x;			// initial position of Mario at scene
	float start_y; 

	float tempx_attack;		//store location x when mario attack in tail level
	float tempy_attack;	//store location y when mario attack in tail level
	
	CEffect* effect;
	vector<CFireBall*> fireballs;
	int fireballindex;
	CTail* tail;
	
	DWORD StartMovingLeft;
	DWORD StartMovingRight;

	DWORD StartRunningLeft;
	DWORD StartRunningRight;

	DWORD StartRunningLeftFast;
	DWORD StartRunningRightFast;

	DWORD StartFly;
	DWORD untouchable_start;
	DWORD FlyFall_start;
	DWORD Attack_start;	
	DWORD Transform_start;

	
public:
	DWORD Jump_start;

	bool untouchable;
	bool isOnGround;
	bool isTransform;
	bool isReadyToHold;
	//bool allowJump;
	bool isRunningRight;
	bool isRunningLeft;

	bool isRunningFastRight;
	bool isRunningFastLeft;

	bool isDucking;
	bool isCrossingPipe;
	bool isFlying;						//for identifying sprites when flying and calculate fly time
	bool isFlyFall;						//for choosing the right sprites for jump and fall fly state	
	bool isAttacking;
	bool isHoldingKoopas;
	bool isJumping;

	bool isClearingCourse;
	bool isInsidePlayScence;

	CMario(float x = 0.0f, float y = 0.0f);
	~CMario();
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
	void Attack();
	void ChangeDirection();

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

	void PushFireBall(CFireBall* FireBall) { fireballs.push_back(FireBall); }
	void SetTail(CTail* t) { tail = t; }
	CTail* GetTail() { return tail; }
	vector<CFireBall*> GetFireBalls() { return fireballs; }
	void SetEffect(CEffect* ef) { effect = ef; }
	CEffect* GetEffect() { return effect; }

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	int GetLevel() { return level; }
	int Getnx() { return nx; }

	void StartTransForm() { isTransform = true; Transform_start = (DWORD)GetTickCount64(); }
	void StartUntouchable() { untouchable = true; untouchable_start = (DWORD)GetTickCount64(); }
	void StartFlyFall() { isFlyFall = true; FlyFall_start = (DWORD)GetTickCount64(); }
	void StartAttack();
	void StartJump();

	void ResetFire();
	void ResetBig();
	void ResetSmall();
	void ResetTail();
	void GoUnderGround();
	void GoHiddenDoor();
	void GoToRunWay();

	void GetBoundingBoxTailLevel(float& left, float& top, float& right, float& bottom);
	void GetBoundingBoxTailWhileAttacking(float& left, float& top, float& right, float& bottom);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};