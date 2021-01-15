#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Mario.h"
#include "Terrain.h"
#include "Camera.h"
#include "IntroNumber.h"
#include "Pointer.h"

class CSceneIntro :public CScene
{
protected:
	CPointer* pointer;
	vector<LPGAMEOBJECT> objects;
	vector<LPTERRAIN> terrains;


	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TERRAIN(string line);
public:
	CSceneIntro(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	CPointer* GetPointer() { return pointer; }

	//friend class CSceneIntroKeyHandler;
};


class CSceneIntroKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CSceneIntroKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
