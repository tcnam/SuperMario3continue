#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Mario.h"
#include "Terrain.h"
#include "Camera.h"
#include "ScoreBoard.h"
#include "Hud.h"
#include "Font.h"
#include "Square.h"
#include "Portal.h"
class CWorldScence : public CScene
{
protected:
	CMario* player;					// A play scene has to have player, right? 
	vector<CPortal*> portals;
	vector<LPGAMEOBJECT> objects;
	vector<LPTERRAIN> terrains;
	vector<CMysteryPiece*> PiecesOfSquare;
	int PiecesIndex = 0;
	Camera* camera;
	CHud* Hud;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TERRAIN(string line);
	void _ParseSection_HUD(string line);
public:
	CWorldScence(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	//friend class CWorldScenceKeyHandler;
};

class CWorldScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CWorldScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
