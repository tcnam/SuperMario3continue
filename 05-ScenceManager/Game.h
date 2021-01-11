#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#define DIRECTINPUT_VERSION 0x0800
#define SCREEN_WIDTH  272.0f
#define SCREEN_HEIGHT 256.0f
#define MAP_WIDTH	2816.0f
#define MAP_HEIGHT	448.0f
#define onesec		1000
#include <dinput.h>

#include "Scence.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int Coins = 0;
	int Scores = 0;
	int Time = 0;
	int life = 4;
	int World = 1;

	int screen_width;
	int screen_height; 

	int map_width;
	int map_height;

	unordered_map<int, LPSCENE> scenes;
	int current_scene; 

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:
	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	int GetMapWidth() { return map_width; }
	int GetMapHeight() { return map_height; }

	int GetCoins() { return Coins; }
	void SetCoins(int n) { Coins = n; }

	int GetScores() { return Scores; }
	void SetScores(int n) { Scores = n; }

	int GetLife() { return life; }
	void SetLife(int n) { life = n; }

	int GetTime() { return Time; }
	void SetTime(int n) { Time = n; }

	int GetWorld() { return World; }
	void SetWorld(int n) { World = n; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);
	static bool AABBCheck(
		float l1,
		float t1,
		float r1,
		float b1,
		float l2,
		float t2,
		float r2,
		float b2
		);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }

	static CGame * GetInstance();

	~CGame();
};


