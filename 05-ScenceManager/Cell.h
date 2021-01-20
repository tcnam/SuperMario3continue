#pragma once
#include "GameObject.h"

class CCell
{
	vector<LPGAMEOBJECT> listObjects;
public:
	void Add(LPGAMEOBJECT obj);
	vector<LPGAMEOBJECT> GetListObjects();
	void Unload();
};

typedef CCell* LPCELL;
