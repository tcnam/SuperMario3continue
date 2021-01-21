#pragma once
#include "Grid.h"
#include "PlayScence.h"
CGrid::CGrid(LPCWSTR filePath)
{
	this->filepath = filePath;
}
CGrid::CGrid()
{
	Rows = Collums = 0;
	cellWidth = cellHeight = 0;
}
CGrid::~CGrid()
{

}
void CGrid::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 4)return;
	cellWidth = atoi(tokens[0].c_str());
	cellHeight = atoi(tokens[1].c_str());
	Collums = atoi(tokens[2].c_str());
	Rows = atoi(tokens[3].c_str());

	cells = new LPCELL[Collums];
	for (int i = 0; i < Collums; i++)
	{
		cells[i] = new CCell[Rows];
	}
}
void CGrid::_ParseSection_OBJECTS(string line, vector<CGameObject*> &Objects)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 4) return;
	int id = atoi(tokens[0].c_str());
	int type = atoi(tokens[1].c_str());
	float x = (float)atof(tokens[2].c_str());
	float y = (float)atof(tokens[3].c_str());

	int cellX = abs(x) / cellWidth;
	int cellY = abs(y) / cellHeight;
	Objects[id]->SetOrigin(x, y);
	cells[cellX][cellY].Add(Objects[id]);

}
void CGrid::Load(vector<LPGAMEOBJECT> &Objects)
{
	ifstream f;
	f.open(filepath);
	int section;

	char str[MAX_GRID_LINE];
	while (f.getline(str, MAX_GRID_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") {
			section = GRID_SECTION_SETTINGS; continue;
		}
		if (line == "[OBJECTS]") {
			section = GRID_SECTION_OBJECTS; continue;
		}
		//
		// data section
		//
		switch (section)
		{
		case GRID_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GRID_SECTION_OBJECTS: _ParseSection_OBJECTS(line,Objects); break;
		}
	}

	f.close();
}
void CGrid::GetListObject(vector<LPGAMEOBJECT>&listObject, Camera* camera)
{
	int left, top, right, bottom;
	int i, j, k;
	float cam_x, cam_y;
	camera->GetPosition(cam_x, cam_y);
	left = cam_x / cellWidth;
	right = (cam_x + CAM_X_IN_USE+1) / cellWidth;
	top = abs(cam_y) / cellHeight;
	bottom = abs(cam_y + CAM_Y_IN_USE) / cellHeight;
	if (right<0 || left>Collums || bottom<0 && top>Rows)
	{
		return;
	}
	if (right > Collums)
	{
		right = Collums;
	}
	if (bottom < 0)
	{
		bottom = 0;
	}
	if (left < 0)
	{
		left = 0;
	}
	if (top > Rows)
	{
		top = Rows;
	}
	for (i = left; i < right; i++)
	{
		for (j = bottom; j < bottom; j++)
		{
			if (!cells[i][j].GetListObjects().empty())
			{
				for (k = 0; k < cells[i][j].GetListObjects().size(); k++)
				{
					if (!cells[i][j].GetListObjects().at(k)->isActived)
					{
						float Ix, Iy;
						cells[i][j].GetListObjects().at(k)->GetOrigin(Ix, Iy);
						if (!((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->IsInUseArea(Ix, Iy))
						{
							if (cells[i][j].GetListObjects().at(k)->GetType() == OBJECT_TYPE_KOOPAS
								&& cells[i][j].GetListObjects().at(k)->GetState() != KOOPAS_STATE_KICKOUT)
							{
								cells[i][j].GetListObjects().at(k)->Reset();
							}
							else if (cells[i][j].GetListObjects().at(k)->GetType() == OBJECT_TYPE_GOOMBA
								&& cells[i][j].GetListObjects().at(k)->GetState() == GOOMBA_STATE_WALKING)
							{
								cells[i][j].GetListObjects().at(k)->Reset();
							}
							listObject.push_back(cells[i][j].GetListObjects().at(k));
							cells[i][j].GetListObjects().at(k)->SetIsActived(true);
						}
					}
				}
			}
		}
	}
}
void CGrid::Unload()
{
	if (this != nullptr)
	{
		if (cells)
		{
			for (int i = 0; i < Collums; i++)
				for (int j = 0; j < Rows; j++)
					cells[i][j].Unload();
			delete cells;
			cells = NULL;
		}
	}
}
