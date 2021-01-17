#pragma once
#include <iostream>
#include <fstream>
#include<stdio.h>
#include<stdlib.h>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Terrain.h"
//#include"Camera.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
	SpecialBrick = NULL;
	player = NULL;
	Hud = NULL;
	camera = NULL;
	tCount = 0;
	MysteryPiece = NULL;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_TERRAIN	7
#define SCENE_SECTION_HUD	8
#define SCENE_SECTION_EFFECT	9
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (unsigned int i = 1; i < tokens.size()-1; i =i+ 2)	// why i+=2 ?  sprite_id | frame_time  tokens.size()-1
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

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
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 4) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();
	CGameObject *obj = NULL;
	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj; 
		player->isInsidePlayScence = true;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA:
		{
			int goomba_level = atoi(tokens[4].c_str());
			obj = new CGoomba();
			((CGoomba*)obj)->SetMario(player);
			((CGoomba*)obj)->Setlevel(goomba_level);
			((CGoomba*)obj)->SetInitPosition(x, y);
		}		
		break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_FRAGMENT: 
		{
			obj = new CFragment();
			((CFragment*)obj)->SetInitPosition(x, y);
			int index_Of_WeakBrick = atoi(tokens[4].c_str());
			if (index_Of_WeakBrick == 18)//Special Brick
				SpecialBrick->PushFragment((CFragment*)obj);
			else
				WeakBricks[index_Of_WeakBrick]->PushFragment((CFragment*)obj);
			DebugOut(L"Fragment has been pushed to WeakBrick with index:%i\n", index_Of_WeakBrick);
		}
		break;
	case OBJECT_TYPE_WEAKBRICK: 
		{
			obj = new CWeakBrick();
			((CWeakBrick*)obj)->SetInitPosition(x, y);
			WeakBricks.push_back((CWeakBrick*)obj);
			DebugOut(L"Weak Brick with index:%i has been pushed to list weakBricks\n", WeaKBrickIndex);
			WeaKBrickIndex++;
		}
		break;
	case OBJECT_TYPE_SPECIALBRICK:
		{
			obj = new CSpecialBrick();
			((CSpecialBrick*)obj)->SetInitPosition(x, y);
			SpecialBrick = (CSpecialBrick*)obj;
		}
		break;
	case OBJECT_TYPE_COIN: 
		{
			obj = new CCoin();
			((CCoin*)obj)->SetMario(player);
			int index_Of_WeakBrick = atoi(tokens[4].c_str());
			if (index_Of_WeakBrick != -1)
			{
				WeakBricks[index_Of_WeakBrick]->PushCoin((CCoin*)obj);
				((CCoin*)obj)->isInsideWeakBrick = true;
			}				
		}		
		break;
	case OBJECT_TYPE_MYSTERYPIECE:
		{
			obj = new CMysteryPiece();
			((CMysteryPiece*)obj)->SetMario(player);			
			if (x == 0)
			{
				PiecesOfSquare.push_back((CMysteryPiece*)obj);
				((CMysteryPiece*)obj)->isFinished = true;
			}				
			else
			{
				((CMysteryPiece*)obj)->SetState(MYSTERYPIECE_STATE_MUSHROOM);		
				MysteryPiece = (CMysteryPiece*)obj;
			}
			
		}
		break;
	case OBJECT_TYPE_MESSAGE:
	{
		obj = new CMessage();
		MysteryPiece->SetMessage((CMessage*)obj);

	}
	break;
	case OBJECT_TYPE_BOUNTYBUTTON:
		{
			obj = new CBountyButton();
			((CBountyButton*)obj)->SetMario(player);			
			SpecialBrick->SetBountyButton((CBountyButton*)obj);
			((CBountyButton*)obj)->isInsideSpecialBrick = true;
			for (unsigned int i = 0; i < WeakBricks.size(); i++)
			{
				((CBountyButton*)obj)->PushWeakBrick(WeakBricks[i]);
			}
		}
		break;
	case OBJECT_TYPE_BOUNTYBRICK: 
		{
			obj = new CBountyBrick();
			((CBountyBrick*)obj)->SetInitPosition(x, y);
			((CBountyBrick*)obj)->SetMario(player);
			bountybricks.push_back((CBountyBrick*)obj);
		}		
		break;
	case OBJECT_TYPE_FIREFLOWER:
		{
			obj = new CFireFlower();
			((CFireFlower*)obj)->SetMario(player);
			((CFireFlower*)obj)->SetInitPosition(x,y);
			FireFlowers.push_back((CFireFlower*)obj);
		}
		break;
	case OBJECT_TYPE_PIRANHA:
		{
			obj = new CPiranha();
			((CPiranha*)obj)->SetMario(player);
			((CPiranha*)obj)->SetInitPosition(x, y);
		}
		break;
	case OBJECT_TYPE_FLOWER_FIREBALL:
		{
			obj = new CFireBallFlower();
			((CFireBallFlower*)obj)->SetMario(player);
			DebugOut(L"flower index: %i\n", FlowerIndex);
			FireFlowers[FlowerIndex]->SetFireBallFlower((CFireBallFlower*)obj);
			DebugOut(L"flower fire ball was loaded\n");
			FlowerIndex++;
		}		
		break;
	case OBJECT_TYPE_BOUNTY:
		{
			obj = new CBounty();
			((CBounty*)obj)->SetMario(player);
			bountybricks[BountyBrickIndex]->SetBounty((CBounty*)obj);
			if (BountyBrickIndex == 3 || BountyBrickIndex == 5 || BountyBrickIndex == 7)
			{
				DebugOut(L"State of powerup%i\n", BOUNTY_STATE_POWERUP);
				bountybricks[BountyBrickIndex]->SetStateBounty(BOUNTY_STATE_POWERUP);
				DebugOut(L"Bounty was added to bountybrick:%i with state:%i\n", BountyBrickIndex, obj->GetState());
			}
			else if (BountyBrickIndex == 8)
			{
				DebugOut(L"State of lifeup%i\n", BOUNTY_STATE_LIFEUP);
				bountybricks[BountyBrickIndex]->SetStateBounty(BOUNTY_STATE_LIFEUP);
				DebugOut(L"Bounty was added to bountybrick:%i with state:%i\n", BountyBrickIndex, obj->GetState());
			}
			else
				bountybricks[BountyBrickIndex]->SetStateBounty(BOUNTY_STATE_COIN);
			bountybricks[BountyBrickIndex]->GetBounty()->SetInitPosition(x, y);
			BountyBrickIndex++;
		}		
		break;
	case OBJECT_TYPE_FIREBALL:
		{	
			obj = new CFireBall();
			player->SetFireBall((CFireBall*)obj);
		}
		break;
	case OBJECT_TYPE_TAIL:
		{
			obj = new CTail();
			player->SetTail((CTail*)obj);
		}
		break;
	case OBJECT_TYPE_KOOPAS:
		{
			int koopas_level = atoi(tokens[4].c_str());
			obj = new CKoopas();
			((CKoopas*)obj)->SetMario(player);
			((CKoopas*)obj)->Setlevel(koopas_level);
			((CKoopas*)obj)->SetInitPosition(x, y);
		}		
		break;
	case OBJECT_TYPE_HIDDENOBJECT: 
		{
			int width = atoi(tokens[4].c_str());
			int height = atoi(tokens[5].c_str());
			obj = new CHiddenObject(x, y, width, height);
		}
		break;
	case OBJECT_TYPE_HIDDENDOOR:
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		int upOrdown = atoi(tokens[6].c_str());
		obj = new CHiddenDoor(x, y, width, height);
		DebugOut(L"Hidden door was loaded\n");
		((CHiddenDoor*)obj)->SetMario(player);
		if(upOrdown==0)
			((CHiddenDoor*)obj)->UpOrDown = false;
		else
			((CHiddenDoor*)obj)->UpOrDown = true;
	}
		break;
	case OBJECT_SYPE_AUTODOOR:
	{
		int width = atoi(tokens[4].c_str());
		int height = atoi(tokens[5].c_str());
		float des_x = (float)atof(tokens[6].c_str());
		float des_y = (float)atof(tokens[7].c_str());
		obj = new CAutoDoor(x, y, width, height,des_x,des_y);
		DebugOut(L"Hidden door was loaded\n");
		((CAutoDoor*)obj)->SetMario(player);
	}
		break;
	case OBJECT_TYPE_PORTAL:
		{	
			float r = (float)atof(tokens[4].c_str());
			float b = (float)atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
			((CPortal*)obj)->SetMario(player);	
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
void CPlayScene::_ParseSection_TERRAIN(string line)
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
void CPlayScene::_ParseSection_EFFECT(string line)
{
	vector<string> tokens = split(line);
	if (tokens.size() < 3) return;
	int owner_id = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());
	int ani_set_id = atoi(tokens[3].c_str());
	CEffect* effect = new CEffect();
	effect->SetOwnerId(owner_id);
	effect->SetPosition(x, y);
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	effect->SetAnimationSet(ani_set);
	if (owner_id == EFFECT_OWNER_ID_MARIO)
		player->SetEffect(effect);
	else if (owner_id == EFFECT_OWNER_ID_FIREFLOWER)
	{
		if (FlowerIndex >= FireFlowers.size())
			FlowerIndex = 0;
		FireFlowers[FlowerIndex]->SetEffect(effect);
		FlowerIndex++;		
	}
	effects.push_back(effect);

}
void CPlayScene::_ParseSection_HUD(string line)
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
			for (unsigned int i = 0; i < PiecesOfSquare.size(); i++)
				square->PushVectorPieces(PiecesOfSquare[i]);
			//square->SetPieceStatesBasedOnGame();
			square->PushMysteryPiece(MysteryPiece);
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

void CPlayScene::Load()
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
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line == "[TERRAINS]"){
			section = SCENE_SECTION_TERRAIN; continue;}
		if (line == "[SCOREBOARD]") {
			section = SCENE_SECTION_HUD; continue;}
		if (line == "[EFFECT]") {
			section = SCENE_SECTION_EFFECT; continue;}
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
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line);break;
			case SCENE_SECTION_TERRAIN: _ParseSection_TERRAIN(line); break;
			case SCENE_SECTION_HUD: _ParseSection_HUD(line); break;
			case SCENE_SECTION_EFFECT: _ParseSection_EFFECT(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	camera = new Camera();
	camera->SetMario(player);	
	Hud->SetCamera(camera);
	CGame::GetInstance()->SetTime(300);
	tCount = DWORD(0);
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a moref organized way 
	
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL)
		return;
	if (tCount == 0)
		tCount = (DWORD)GetTickCount64();
	vector<LPGAMEOBJECT> coObjects;
	vector<LPGAMEOBJECT> coObjects_Of_FireFlower_Coin_FireBallFlower_MysteryPiece;			//1: List of collidable Objects of FireFlower(or Coin, or FireBallFlower)
	vector<LPGAMEOBJECT> coObjects_Of_Bounty;									//2: List of collidable Objects of Bounty
	vector<LPGAMEOBJECT> coObjects_Of_Goomba;									//3: List of collidable Objects of Goomba
	vector<LPGAMEOBJECT> coObjects_Of_Koopas;									//4: List of collidable Objects of Koopas
	vector<LPGAMEOBJECT> coObbjects_Of_FireBall;								//5: List of collidable Objects of FireBall
	vector<LPGAMEOBJECT> coObjects_Of_Mario;									//6: List of collidable Objects of Mario
	vector<LPGAMEOBJECT> coObjects_Of_BountyBrick_WeakBrick;								//7: List of collidable Objects of BountyBrick
	vector<LPGAMEOBJECT> coObjects_Of_Tail;



	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->type == OBJECT_TYPE_MARIO)				//1,2,3,4,7
		{
			if (((CMario*)objects[i])->untouchable == true)
			{
				coObjects_Of_FireFlower_Coin_FireBallFlower_MysteryPiece.push_back(objects[i]);
				coObjects_Of_Bounty.push_back(objects[i]);
				coObjects_Of_BountyBrick_WeakBrick.push_back(objects[i]);
			}
			else
			{
				coObjects_Of_FireFlower_Coin_FireBallFlower_MysteryPiece.push_back(objects[i]);
				coObjects_Of_Bounty.push_back(objects[i]);
				coObjects_Of_BountyBrick_WeakBrick.push_back(objects[i]);
				coObjects_Of_Goomba.push_back(objects[i]);
				coObjects_Of_Koopas.push_back(objects[i]);
			}
		}
		else if (objects[i]->type == OBJECT_TYPE_KOOPAS)			//4,5,7
		{
			coObjects_Of_Tail.push_back(objects[i]);
			coObjects_Of_Koopas.push_back(objects[i]);
			coObbjects_Of_FireBall.push_back(objects[i]);
			coObjects_Of_BountyBrick_WeakBrick.push_back(objects[i]);
			if (player->untouchable == false)
				coObjects_Of_Mario.push_back(objects[i]);
		}
		else if (objects[i]->type == OBJECT_TYPE_GOOMBA)		//3,4,5
		{
			coObjects_Of_Tail.push_back(objects[i]);
			coObjects_Of_Goomba.push_back(objects[i]);
			coObjects_Of_Koopas.push_back(objects[i]);
			coObbjects_Of_FireBall.push_back(objects[i]);
			if (player->untouchable == false)
				coObjects_Of_Mario.push_back(objects[i]);
		}
		else if (objects[i]->type == OBJECT_TYPE_FIREFLOWER)	//4,5
		{
			coObjects_Of_Tail.push_back(objects[i]);
			coObjects_Of_Koopas.push_back(objects[i]);
			coObbjects_Of_FireBall.push_back(objects[i]);
		}
		else if (objects[i]->type == OBJECT_TYPE_BRICK || objects[i]->type == OBJECT_TYPE_HIDDENOBJECT)		//2,3,4,5,6
		{
			coObjects_Of_Bounty.push_back(objects[i]);
			coObjects_Of_Goomba.push_back(objects[i]);
			coObjects_Of_Koopas.push_back(objects[i]);
			coObbjects_Of_FireBall.push_back(objects[i]);
			coObjects_Of_Mario.push_back(objects[i]);
		}
		else if (objects[i]->type == OBJECT_TYPE_BOUNTYBRICK)					//2,3,5
		{
			coObjects_Of_Tail.push_back(objects[i]);
			coObjects_Of_Bounty.push_back(objects[i]);
			coObjects_Of_Goomba.push_back(objects[i]);
			coObbjects_Of_FireBall.push_back(objects[i]);
			coObjects_Of_Mario.push_back(objects[i]);					//add bountybrick to coOjects of Mario to block mario
			coObjects_Of_Koopas.push_back(objects[i]);					//add bountybrick to coOjects of Koopas to block Koopas
		}
		else if (objects[i]->type == OBJECT_TYPE_WEAKBRICK)						//2,3,5
		{
			coObjects_Of_Tail.push_back(objects[i]);
			coObjects_Of_Bounty.push_back(objects[i]);
			coObjects_Of_Goomba.push_back(objects[i]);
			coObbjects_Of_FireBall.push_back(objects[i]);
			coObjects_Of_Mario.push_back(objects[i]);					//add weakbrick to coOjects of Mario to block mario
			coObjects_Of_Koopas.push_back(objects[i]);					//add weakbrick to coOjects of Koopas to block Koopas
		}
		else if (objects[i]->type == OBJECT_TYPE_SPECIALBRICK)						//2,3,5
		{
			coObjects_Of_Tail.push_back(objects[i]);
			coObjects_Of_Bounty.push_back(objects[i]);
			coObjects_Of_Goomba.push_back(objects[i]);
			coObbjects_Of_FireBall.push_back(objects[i]);
			coObjects_Of_Mario.push_back(objects[i]);					//add specialbrick to coOjects of Mario to block mario
			coObjects_Of_Koopas.push_back(objects[i]);					//add specialbrick to coOjects of Koopas to block Koopas
		}
		else if (objects[i]->type == OBJECT_TYPE_PIRANHA)
		{
			coObjects_Of_Tail.push_back(objects[i]);
		}
		//else if (objects[i]->type == OBJECT_TYPE_PORTAL)
		//{
		//	coObjects_Of_Mario.push_back(objects[i]);
		//}
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->type == OBJECT_TYPE_FLOWER_FIREBALL
			|| objects[i]->type == OBJECT_TYPE_COIN
			|| objects[i]->type == OBJECT_TYPE_FIREFLOWER
			|| objects[i]->type == OBJECT_TYPE_BOUNTYBUTTON
			|| objects[i]->type == OBJECT_TYPE_MYSTERYPIECE)
			objects[i]->Update(dt, &coObjects_Of_FireFlower_Coin_FireBallFlower_MysteryPiece);

		else if (objects[i]->type == OBJECT_TYPE_BOUNTY)
			objects[i]->Update(dt, &coObjects_Of_Bounty);

		else if (objects[i]->type == OBJECT_TYPE_GOOMBA)
			objects[i]->Update(dt, &coObjects_Of_Goomba);

		else if (objects[i]->type == OBJECT_TYPE_KOOPAS)
			objects[i]->Update(dt, &coObjects_Of_Koopas);

		else if (objects[i]->type == OBJECT_TYPE_FIREBALL)
			objects[i]->Update(dt, &coObbjects_Of_FireBall);

		else if (objects[i]->type == OBJECT_TYPE_MARIO)
			objects[i]->Update(dt, &coObjects_Of_Mario);

		else if (objects[i]->type == OBJECT_TYPE_BOUNTYBRICK || objects[i]->type == OBJECT_TYPE_WEAKBRICK || objects[i]->type == OBJECT_TYPE_SPECIALBRICK)
			objects[i]->Update(dt, &coObjects_Of_BountyBrick_WeakBrick);
		else if (objects[i]->type == OBJECT_TYPE_TAIL)
			objects[i]->Update(dt, &coObjects_Of_Tail);
		else
		{
			objects[i]->Update(dt, &coObjects);			
		}
	}
	if (player == NULL)
		return;
	for (unsigned int i = 0; i < effects.size(); i++)
	{
		if (effects[i] == NULL)
			continue;
		effects[i]->Update(dt);
	}
	// Update camera to follow mario
	camera->Update();
	//when mario dropping out from camera vision will triggers unload scence
	if (player == NULL)				
		return;
	Hud->Update();
	if(player->isTransform==false)
		TimeCount();
}

void CPlayScene::Render()
{
	for (unsigned int i = 0; i < terrains.size(); i++)
		terrains[i]->Draw(terrains[i]->GetPositionX(),terrains[i]->GetPositionY(),255);
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
	for (unsigned int i = 0; i < effects.size(); i++)
	{
		effects[i]->Render();
	}
	Hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (unsigned int i = 0; i < effects.size(); i++)
		delete effects[i];
	effects.clear();
	player = NULL;
	SpecialBrick = NULL;
	camera = NULL;
	Hud = NULL;
	tCount = 0;	
	MysteryPiece = NULL;
	BountyBrickIndex = 0;
	FlowerIndex = 0;
	WeaKBrickIndex = 0;
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i] != NULL)
		{
			float x, y;
			objects[i]->GetPosition(x, y);
			DebugOut(L"type of the object:%i\n with x= %f and y= % f\n", objects[i]->GetType(), x, y);
			delete objects[i];
		}					
	}
		
	for (unsigned int i = 0; i < terrains.size(); i++)
		delete terrains[i];
	
	FireFlowers.clear();
	WeakBricks.clear();
	bountybricks.clear();
	objects.clear();
	terrains.clear();
	
	PiecesOfSquare.clear();
	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CPlayScene::TimeCount()
{
	if (GetTickCount64() - tCount >= 1000)
	{
		tCount = 0;
		if (CGame::GetInstance()->GetTime() == 1)
		{
			player->SetState(MARIO_STATE_DIE);
			player->SetSpeed(0, -MARIO_DIE_DEFLECT_SPEED);
		}
		else
		{
			CGame::GetInstance()->SetTime(CGame::GetInstance()->GetTime() - 1);
		}
		
	}
}
void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE || mario->isClearingCourse == true||mario->isTransform==true || mario->isCrossingPipe == true)
		return;
	switch (KeyCode)
	{
	case DIK_S:
		//mario->allowJump = false;
		{
			if (mario->GetLevel() == MARIO_LEVEL_TAIL)				//tail mario
			{
				if (mario->isRunningFastLeft == true)
				{
					if (mario->isFlying == false)			//set time to start flying 
					{
						mario->SetTimeFly((DWORD)GetTickCount64());
						mario->isFlying = true;
					}
					if ((DWORD)GetTickCount64() - mario->GetTimeFly() < MARIO_FLY_TIME)
					{
						mario->SetState(MARIO_STATE_FLYLEFT);
					}
					else
					{
						if(mario->isDucking==false)
							mario->StartFlyFall();
					}
				}
				else if (mario->isRunningFastRight == true)
				{
					if (mario->isFlying == false)			//set time to start flying 
					{
						mario->SetTimeFly((DWORD)GetTickCount64());
						mario->isFlying = true;
					}
					if ((DWORD)GetTickCount64() - mario->GetTimeFly() < MARIO_FLY_TIME)
					{
						mario->SetState(MARIO_STATE_FLYRIGHT);
					}
					else
					{
						if (mario->isDucking == false)
							mario->StartFlyFall();
					}
				}
				else
				{
					if (mario->isOnGround == true)
					{
						mario->SetState(MARIO_STATE_JUMP);
						mario->StartJump();
					}
						
					else
					{
						if (mario->isDucking == false)
							mario->StartFlyFall();
					}
				}
			}
			else
			{
				mario->SetState(MARIO_STATE_JUMP);
				mario->StartJump();
			}
		}
		
		break;
	case DIK_1:
		mario->ResetSmall();
		break;
	case DIK_2:
		mario->ResetBig();
		break;
	case DIK_3:
		mario->ResetTail();
		break;
	case DIK_4:
		mario->ResetFire();
		break;
	case DIK_5:
		mario->GoUnderGround();
		break;
	case DIK_6:
		mario->GoHiddenDoor();
		break;
	case DIK_7:
		mario->GoToRunWay();
		break;
	case DIK_A:
		if (mario->isDucking == true)
			return;
		if (mario->isAttacking == true)
			return;
		else
		{
			if (mario->GetLevel() == MARIO_LEVEL_FIRE)
			{
				mario->StartAttack();
				mario->SetState(MARIO_STATE_ATTACK);
			}
			else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			{
				mario->StartAttack();
				mario->SetState(MARIO_STATE_ATTACK);
			}
		}		
		break;
	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario* mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE || mario->isClearingCourse == true||mario->isTransform==true||mario->isCrossingPipe==true)
		return;
	switch (KeyCode)
	{
	/*case DIK_RIGHT:
		mario->StartSlide();
		mario->SetState(MARIO_STATE_SLIDE_RIGHT);
		break;
	case DIK_LEFT:
		mario->StartSlide();
		mario->SetState(MARIO_STATE_SLIDE_LEFT);
		break;*/
	case DIK_A:
	{
		if (mario->isHoldingKoopas == true)
			mario->isHoldingKoopas = false;
		if (mario->isRunningLeft == true)
			mario->isRunningLeft = false;
		if (mario->isRunningRight == true)
			mario->isRunningRight = false;
		if (mario->isReadyToHold == true)
			mario->isReadyToHold = false;
	}
		
		break;
	case DIK_DOWN:
	{
		if (mario->isDucking == true)
			mario->isDucking = false;
		float current_x, current_y;
		mario->GetPosition(current_x, current_y);
		mario->SetPosition(current_x, current_y - 10);
	}
	break;
	case DIK_S:
	{
		if (mario->isJumping == true)
		{
			mario->isJumping = false;
			mario->Jump_start = 0;
		}
	}
	break;
	
	}
}


void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	// disable control key when Mario die 
	
	if (mario->GetState() == MARIO_STATE_DIE||mario->isClearingCourse==true||mario->isTransform==true || mario->isCrossingPipe == true)
		return;
	if (game->IsKeyDown(DIK_LEFT) && !game->IsKeyDown(DIK_RIGHT)&&!game->IsKeyDown(DIK_DOWN))
	{
		if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->nx > 0)
			mario->x = mario->x + 7;
		mario->SetTimeMovingLeft(DWORD(GetTickCount64()));
		if (DWORD(GetTickCount64()) - mario->GetTimeMovingRight() > MARIO_TIME_CHANGE_DIRECTION)
		{
			if (game->IsKeyDown(DIK_A)&&game->IsKeyDown(DIK_LEFT)&&mario->isHoldingKoopas==false)
			{
				if (mario->isRunningLeft == false)
					mario->SetTimeRunningLeft((DWORD)GetTickCount64());
				if ((DWORD)GetTickCount64() - mario->GetTimeRunningLeft() > MARIO_RUNTIME)
				{
					mario->isRunningLeft = true;
					mario->isRunningFastLeft = true;
					mario->SetState(MARIO_STATE_RUNNINGFAST_LEFT);
				}
				else
				{
					mario->isRunningLeft = true;
					mario->SetState(MARIO_STATE_RUNNING_LEFT);					
				}
			}
			else
			{
				mario->SetState(MARIO_STATE_WALKING_LEFT);
			}	
			
		}
		else
		{
			if (mario->isFlying == false)
			{
				mario->SetState(MARIO_STATE_CHANGELEFT);
				mario->isRunningRight = false;
				mario->isRunningFastRight = false;

			}			
		}

	}
	else if (game->IsKeyDown(DIK_RIGHT) && !game->IsKeyDown(DIK_LEFT)&&!game->IsKeyDown(DIK_DOWN))
	{
		if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->nx < 0)
			mario->x = mario->x - 7;
		mario->SetTimeMovingRight(DWORD(GetTickCount64()));
		
		if (DWORD(GetTickCount64()) - mario->GetTimeMovingLeft() > MARIO_TIME_CHANGE_DIRECTION)
		{
			if (game->IsKeyDown(DIK_A)&&game->IsKeyDown(DIK_RIGHT)&&mario->isHoldingKoopas==false)
			{
				if (mario->isRunningRight == false)
					mario->SetTimeRunningRight((DWORD)GetTickCount64());
				if ((DWORD)GetTickCount64() - mario->GetTimeRuningRight() > MARIO_RUNTIME)
				{
					mario->isRunningRight = true;
					mario->isRunningFastRight = true;
					mario->SetState(MARIO_STATE_RUNNINGFAST_RIGHT);
				}
				else
				{
					mario->isRunningRight = true;
					mario->SetState(MARIO_STATE_RUNNING_RIGHT);
				}
			}
			else
				mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		else
		{
			if (mario->isFlying == false)
			{
				mario->SetState(MARIO_STATE_CHANGERIGHT);
				mario->isRunningFastLeft = false;
				mario->isRunningLeft = false;
			}			
		}
		//if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->nx < 0)
		//	mario->x = mario->x - MARIO_TAIL_ATTACK_BBOX_WIDTH + MARIO_TAIL_BBOX_WIDTH;
	}
	else if (game->IsKeyDown(DIK_DOWN))
	{
		mario->SetState(MARIO_STATE_DUCK);
		mario->isDucking = true;
	}
	else if (game->IsKeyDown(DIK_A)&&!game->IsKeyDown(DIK_LEFT)&&!game->IsKeyDown(DIK_RIGHT)&&!game->IsKeyDown(DIK_DOWN))
	{
		mario->isReadyToHold = true;
	}
	else
	{
		mario->isReadyToHold = false;
		mario->isDucking = false;
		mario->SetState(MARIO_STATE_IDLE);
	}		
}