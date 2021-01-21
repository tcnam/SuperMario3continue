#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "BountyBrick.h"
#include "HiddenObject.h"
#include "FireBall.h"
#include "Terrain.h"
#include "FireFlower.h"
#include "FireBallFLower.h"
#include "Coin.h"
#include "Bounty.h"
#include "Fragment.h"
#include "WeakBrick.h"
#include "BountyButton.h"
#include "SpecialBrick.h"
#include "MysteryPiece.h"
#include "Camera.h"
#include "ScoreBoard.h"
#include "Hud.h"
#include "Font.h"
#include "Square.h"
#include "Message.h"
#include "Effect.h"
#include "HiddenDoor.h"
#include "AutoDoor.h"
#include "Tail.h"
#include "Piranha.h"
#include "FlyBrick.h"
#include "BoomerangBro.h"
#include "Boomerang.h"
#include "Grid.h"

class CPlayScene: public CScene
{
protected: 
	CMario *player;					// A play scene has to have player, right? 
	
	vector<CFireFlower*>FireFlowers;
	vector <CBountyBrick*> bountybricks;
	vector <CWeakBrick*> WeakBricks;

	vector<CMysteryPiece*> PiecesOfSquare;

	CSpecialBrick* SpecialBrick;
	CMysteryPiece* MysteryPiece;
	CBoomerangBro* bro;

	vector<LPGAMEOBJECT> objects;
	vector<LPTERRAIN> terrains;
	vector<CEffect*> effects;
	Camera* camera;
	CHud* Hud;
	CGrid* Grid;

	DWORD tCount;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_TERRAIN(string line);
	void _ParseSection_HUD(string line);
	void _ParseSection_EFFECT(string line);
	void _ParseSection_GRID(string line);
public: 
	CPlayScene(int id, LPCWSTR filePath);
	void TimeCount();
	bool IsInUseArea(float x, float y);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//vector<LPGAMEOBJECT>	GetObjects() { return objects; }
	CMario * GetPlayer() { return player; } 

	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

