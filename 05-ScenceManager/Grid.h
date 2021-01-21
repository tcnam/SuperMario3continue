#pragma once
#include "Game.h"
#include <iostream>
#include <fstream>
#include "Cell.h"
#include "Camera.h"
#include "Mario.h"
#include "BountyBrick.h"
#include "FireFlower.h"
#include "WeakBrick.h"
#include "MysteryPiece.h"
#include "SpecialBrick.h"
#include "BoomerangBro.h"
#include "HiddenDoor.h"
#include "AutoDoor.h"
#include "Tail.h"
#include "Piranha.h"
#include "FlyBrick.h"
#include "BoomerangBro.h"
#include "Boomerang.h"
#include "Portal.h"
using namespace std;

#define GRID_SECTION_SETTINGS	1
#define GRID_SECTION_OBJECTS	2
#define MAX_GRID_LINE			1024


class CGrid
{
	CCell** cells;
	int Rows, Collums;
	int cellWidth, cellHeight;
	LPCWSTR filepath;
	
	vector<LPGAMEOBJECT> permanentObjects;
	
	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line, vector<CGameObject*>& Objects);
public:
	CGrid();
	CGrid(LPCWSTR filePath);
	~CGrid();
	void GetListObject(vector<CGameObject*>&Obj, Camera* camera);
	void Load(LPCWSTR filePath,vector<LPGAMEOBJECT> &Objects);
	void Unload();
};
