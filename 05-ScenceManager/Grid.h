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
	
	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line, 
		CMario* player,
		vector<CBountyBrick*>& BBrick,
		vector<CWeakBrick*>&WBrick,
		vector<CFireFlower*>& flowers,
		vector<CMysteryPiece*>& pieces,
		CSpecialBrick* spBrick,
		CBoomerangBro* bro,
		CMysteryPiece* lastpiece
	);
public:
	CGrid(){}
	CGrid(LPCWSTR filePath);
	~CGrid(){}
	void GetListObject(vector<CGameObject*>& listObj, Camera* camera);
	void Load(LPCWSTR filePath);
	void Unload();
};
