#include "Cell.h"
void CCell::Add(CGameObject* obj)
{
	listObjects.push_back(obj);
}

vector<LPGAMEOBJECT> CCell::GetListObjects()
{
	return listObjects;
}


void CCell::Unload()
{
	listObjects.clear();
}