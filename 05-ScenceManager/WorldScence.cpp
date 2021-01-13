#pragma once
#include <iostream>
#include <fstream>
#include<stdio.h>
#include<stdlib.h>

#include "WorldScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Terrain.h"
//#include"Camera.h"

using namespace std;

CWorldScence::CWorldScence(int id, LPCWSTR filePath) :CScene(id, filePath)
{
	key_handler = new CWorldScenceKeyHandler(this);
	player = NULL;
	Hud = NULL;
	camera = NULL;
}
#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TERRAIN	7
#define SCENE_SECTION_HUD	8
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024
void CWorldScence::_ParseSection_TEXTURES(string line)
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

void CWorldScence::_ParseSection_SPRITES(string line)
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

void CWorldScence::_ParseSection_ANIMATIONS(string line)
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

void CWorldScence::_ParseSection_ANIMATION_SETS(string line)
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
void CWorldScence::_ParseSection_OBJECTS(string line)
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
	case OBJECT_TYPE_MARIO:
		{
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new CMario(x, y);
			player = (CMario*)obj;
			player->isInsidePlayScence = false;
			DebugOut(L"[INFO] Player object created!\n");
		}		
			break;
	case OBJECT_TYPE_HIDDENOBJECT:
		{
			int width = atoi(tokens[4].c_str());
			int height = atoi(tokens[5].c_str());
			obj = new CHiddenObject(x, y, width, height);
		}
	break;
	case OBJECT_TYPE_MYSTERYPIECE:
	{
		obj = new CMysteryPiece();
		((CMysteryPiece*)obj)->SetMario(player);
		PiecesOfSquare.push_back((CMysteryPiece*)obj);
		((CMysteryPiece*)obj)->isFinished = true;
		int temp1 = CGame::GetInstance()->GetPiece1();
		int temp2 = CGame::GetInstance()->GetPiece2();
		int temp3 = CGame::GetInstance()->GetPiece3();
		if (PiecesIndex == 0)
			((CMysteryPiece*)obj)->SetState(temp1);
		else if (PiecesIndex == 1)
			((CMysteryPiece*)obj)->SetState(temp2);
		else
			((CMysteryPiece*)obj)->SetState(temp3);
		PiecesIndex++;
		/*else
		{
			((CMysteryPiece*)obj)->SetState(MYSTERYPIECE_STATE_MUSHROOM);
			MysteryPiece = (CMysteryPiece*)obj;
		}*/

	}
	break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[4].c_str());
		float b = (float)atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		((CPortal*)obj)->SetMario(player);
		portals.push_back((CPortal*)obj);
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

	objects.push_back(obj);
}
void CWorldScence::_ParseSection_TERRAIN(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return;
	float x = (float)atof(tokens[0].c_str());
	float y = (float)atof(tokens[1].c_str());
	int sprite_id = atoi(tokens[2].c_str());
	CTerrain* terr = new CTerrain();
	terr->SetPosition(x, y);
	LPSPRITE sprites = CSprites::GetInstance()->Get(sprite_id);
	terr->SetSprite(sprites);
	terrains.push_back(terr);
}
void CWorldScence::_ParseSection_HUD(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 2)return;
	int type = atoi(tokens[0].c_str());
	switch (type)
	{
	case HUD_TYPE_SCOREBOARD:
	{
		int sprite_id = atoi(tokens[1].c_str());
		CScoreBoard* scoreboard = new CScoreBoard();
		LPSPRITE sprites = CSprites::GetInstance()->Get(sprite_id);
		scoreboard->SetSprite(sprites);
		scoreboard->SetMario(player);
		Hud->SetScoreBoard(scoreboard);
	}
	break;
	case HUD_TYPE_SQUARE:
	{
		int sprite_id = atoi(tokens[1].c_str());
		CSquare* square = new CSquare();
		LPSPRITE sprites = CSprites::GetInstance()->Get(sprite_id);
		square->SetSprite(sprites);
		square->SetMario(player);
		//square->SetPieceStatesBasedOnGame();
		Hud->SetSquare(square);
	}
	break;
	case HUD_TYPE_BACKGROUND:
	{
		int sprite_id = atoi(tokens[1].c_str());
		LPSPRITE sprites = CSprites::GetInstance()->Get(sprite_id);
		Hud->SetSprite(sprites);
	}
	break;
	case HUD_TYPE_FONT:
	{
		float x = (float)atof(tokens[1].c_str());
		float y = (float)atof(tokens[2].c_str());
		int type_font = atoi(tokens[3].c_str());
		int ani_set_id = atoi(tokens[4].c_str());
		int font_belong_id = atoi(tokens[5].c_str());
		CFont* font = new CFont();
		CAnimationSets* animation_sets = CAnimationSets::GetInstance();
		font->SetRelativePosition(x, y);
		font->SetType(type_font);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		font->SetAnimationSet(ani_set);
		switch (font_belong_id)
		{
		case -1:
			Hud->GetScoreBoard()->SetBackGroundFont(font);
			break;
		case 0:
			Hud->GetScoreBoard()->SetWorldNumber(font);
			break;
		case 1:
			Hud->GetScoreBoard()->PushSpeedState(font);
			break;
		case 2:
			Hud->GetScoreBoard()->SetMarioSpeedMaxState(font);
			break;
		case 3:
			Hud->GetScoreBoard()->PushCoins(font);
			break;
		case 4:
			Hud->GetScoreBoard()->SetMarioLife(font);
			break;
		case 5:
			Hud->GetScoreBoard()->PushScores(font);
			break;
		case 6:
			Hud->GetScoreBoard()->PushTime(font);
			break;

		}

	}
	break;
	}
}

void CWorldScence::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	Hud = new CHud();
	//Hud = NULL;
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
		if (line == "[SCOREBOARD]") {
			section = SCENE_SECTION_HUD; continue;
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
		case SCENE_SECTION_HUD: _ParseSection_HUD(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	camera = new Camera();
	camera->SetMario(player);
	Hud->SetCamera(camera);
	CGame::GetInstance()->SetTime(0);
	for (unsigned int i = 0; i < PiecesOfSquare.size(); i++)
	{
		DebugOut(L"state:%i\n", PiecesOfSquare[i]->GetState());
	}

	
}
void CWorldScence::Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObbjects_Of_Portals;								//5: List of collidable Objects of FireBall
	vector<LPGAMEOBJECT> coObjects_Of_Mario;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->GetType() == OBJECT_TYPE_MARIO)
		{
			coObbjects_Of_Portals.push_back(objects[i]);
		}
		else if (objects[i]->GetType() == OBJECT_TYPE_PORTAL)
			continue;
		else
			coObjects_Of_Mario.push_back(objects[i]);
	}
	player->Update(dt,&coObjects_Of_Mario);
	for (unsigned i = 0; i < portals.size(); i++)
		portals[i]->Update(dt, &coObbjects_Of_Portals);

	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a moref organized way 
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario
	camera->Update();
	Hud->Update();
}

void CWorldScence::Render()
{
	for (unsigned int i = 0; i < terrains.size(); i++)
		terrains[i]->Draw(round(terrains[i]->GetPositionX()), round(terrains[i]->GetPositionY()), 255);
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
	Hud->Render();
}

/*
	Unload current scene
*/
void CWorldScence::Unload()
{
	camera = NULL;
	Hud = NULL;
	player = NULL;
	portals.clear();
	for (unsigned int i = 0; i < objects.size(); i++)
	{		
			float x, y;
			objects[i]->GetPosition(x, y);
			DebugOut(L"type of the object:%i\n with x= %f and y= % f\n", objects[i]->GetType(), x, y);
			delete objects[i];
	}

	for (unsigned int i = 0; i < terrains.size(); i++)
		delete terrains[i];
	objects.clear();
	terrains.clear();
	PiecesOfSquare.clear();
	PiecesIndex = 0;
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CWorldScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario* mario = ((CWorldScence*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE || mario->isClearingCourse == true)
		return;
	switch (KeyCode)
	{
	case (DIK_A):
		mario->SetState(MARIO_STATE_GOTO_ANOTHER_SCENCE);
		break;
	}
}
void CWorldScenceKeyHandler::OnKeyUp(int KeyCode)
{
	
}


void CWorldScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CMario* mario = ((CWorldScence*)scence)->GetPlayer();

	if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT))
	{
		mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT))
	{
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_UP) && !game->IsKeyDown(DIK_DOWN))
	{
		mario->SetState(MARIO_STATE_MOVE_UP);
	}
	else if (game->IsKeyDown(DIK_DOWN) && !game->IsKeyDown(DIK_UP))
	{
		mario->SetState(MARIO_STATE_MOVE_DOWN);
	}
	else if (!game->IsKeyDown(DIK_SPACE))
	{
		mario->SetState(MARIO_STATE_IDLE);
	}
}