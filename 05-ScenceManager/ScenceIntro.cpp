#pragma once
#include <iostream>
#include <fstream>
#include<stdio.h>
#include<stdlib.h>
#include"SceneIntro.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Terrain.h"
#define MAX_SCENE_LINE 1024
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TERRAIN	7
CSceneIntro::CSceneIntro(int id, LPCWSTR filePath) :CScene(id, filePath)
{
	pointer = NULL;
	IntroMenu = NULL;
	IntroNumber = NULL;
	blackbackground = NULL;
	curtain = NULL;
	key_handler = new CSceneIntroKeyHandler(this);

}
void CSceneIntro::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CSceneIntro::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CSceneIntro::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size() - 1; i = i + 2)	// why i+=2 ?  sprite_id | frame_time  tokens.size()-1
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CSceneIntro::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (unsigned int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}
	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CSceneIntro::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 4) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* obj = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_INTRONUMBER:
		obj = new CIntroNumber();
		IntroNumber = (CIntroNumber*)obj;
		break;
	case OBJECT_TYPE_POINTER:
		{
			obj = new CPointer();
			pointer = (CPointer*)obj;
		}
			break;
	case OBJECT_TYPE_CURTAIN:
	{
		obj = new CCurtain();
		curtain = (CCurtain*)obj;
	}
	break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	obj->SetType(object_type);						//set type for the object
	// General object setup
	obj->SetPosition(x, y);
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);

}
void CSceneIntro::_ParseSection_TERRAIN(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return;
	float x = (float)atof(tokens[0].c_str());
	float y = (float)atof(tokens[1].c_str());
	int sprite_id = atoi(tokens[2].c_str());
	int MenuOrBack = atoi(tokens[3].c_str());
	CTerrain* terr = new CTerrain();
	terr->SetPosition(x, y);
	LPSPRITE sprites = CSprites::GetInstance()->Get(sprite_id);
	terr->SetSprite(sprites);
	if (MenuOrBack == 2)
		blackbackground = terr;
	else
		IntroMenu = terr;
}
void CSceneIntro::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[TERRAINS]") {
			section = SCENE_SECTION_TERRAIN; continue;
		}
		
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TERRAIN: _ParseSection_TERRAIN(line); break;
		
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

}
void CSceneIntro::Update(DWORD dt)
{

	curtain->Update(dt);
	CGame::GetInstance()->SetCamPos(round(0.0f),round(-SCREEN_HEIGHT + 64.0f));
}

void CSceneIntro::Render()
{
	blackbackground->Draw(round(blackbackground->GetPositionX()),round(blackbackground->GetPositionY()),255);

	if (curtain->isFinish == false)
	{
		curtain->Render();
	}
	else
	{		
		IntroMenu->Draw(round(IntroMenu->GetPositionX()), round(IntroMenu->GetPositionY()), 255);
		pointer->Render();
		IntroNumber->Render();
	}
	
}

/*
	Unload current scene
*/
void CSceneIntro::Unload()
{
	IntroMenu = NULL;
	IntroNumber = NULL;
	pointer = NULL;
	curtain = NULL;
	blackbackground = NULL;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CSceneIntroKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CGame* game = CGame::GetInstance();
	CPointer* pointer = ((CSceneIntro*)scence)->GetPointer();
	switch (KeyCode)
	{
	case (DIK_S):
		CGame::GetInstance()->SwitchScene(1);
		break;
	case (DIK_UP):
		pointer->SetPosition(68.0f, -36.0f);
		break;
	case(DIK_DOWN):
		pointer->SetPosition(68.0f, -19.0f);
		break;
	}
}
void CSceneIntroKeyHandler::OnKeyUp(int KeyCode)
{

}
void CSceneIntroKeyHandler::KeyState(BYTE* states)
{
}