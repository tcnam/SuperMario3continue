//#include "Grid.h"
//CGrid::CGrid(LPCWSTR filePath)
//{
//	Load(filePath);
//}
//void CGrid::_ParseSection_SETTINGS(string line)
//{
//	vector<string> tokens = split(line);
//	if (tokens.size() < 4)return;
//	cellWidth = atoi(tokens[0].c_str());
//	cellHeight = atoi(tokens[1].c_str());
//	Collums = atoi(tokens[2].c_str());
//	Rows = atoi(tokens[3].c_str());
//
//	cells = new LPCELL[Collums];
//	for (int i = 0; i < Collums; i++)
//	{
//		cells[i] = new CCell[Rows];
//	}
//}
//void CGrid::_ParseSection_OBJECTS(string line, CMario* player,
//	vector<CBountyBrick*>& BBrick,
//	vector<CWeakBrick*>&WBrick,
//	vector<CFireFlower*>& flowers,
//	vector<CMysteryPiece*>& pieces,
//	CSpecialBrick* spBrick,
//	CBoomerangBro* bro,
//	CMysteryPiece* lastpiece
//	)
//{
//	vector<string> tokens = split(line);
//	if (tokens.size() < 4)return;
//
//	int object_type = atoi(tokens[0].c_str());
//	float x = (float)atof(tokens[1].c_str());
//	float y = (float)atof(tokens[2].c_str());
//	int ani_set_id = atoi(tokens[3].c_str());
//
//	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
//	CGameObject* obj = NULL;
//
//	int cellX = (abs(x) / cellWidth);
//	int cellY = (abs(y) / cellHeight);
//	switch (object_type)
//	{
//	case OBJECT_TYPE_MARIO:
//		if (player != NULL)
//		{
//			DebugOut(L"[ERROR] MARIO object was created before!\n");
//			return;
//		}
//		obj = new CMario(x, y);
//		player = (CMario*)obj;
//		player->isInsidePlayScence = true;
//		DebugOut(L"[INFO] Player object created!\n");
//		break;
//	case OBJECT_TYPE_GOOMBA:
//		{
//			int goomba_level = atoi(tokens[4].c_str());
//			obj = new CGoomba();
//			((CGoomba*)obj)->SetMario(player);
//			((CGoomba*)obj)->Setlevel(goomba_level);
//			((CGoomba*)obj)->SetInitPosition(x, y);
//		}
//		break;
//	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
//	case OBJECT_TYPE_FLYBRICK: obj = new CFlyBrick(); break;
//	case OBJECT_TYPE_FRAGMENT:
//		{
//			obj = new CFragment();
//			((CFragment*)obj)->SetInitPosition(x, y);
//			//int index_Of_WeakBrick = atoi(tokens[4].c_str());
//			//if (index_Of_WeakBrick == 18 && CGame::GetInstance()->GetSceneNumber() == 2)//Special Brick
//			//	spBrick->PushFragment((CFragment*)obj);
//			//else
//			//	WBrick[index_Of_WeakBrick]->PushFragment((CFragment*)obj);
//			//DebugOut(L"Fragment has been pushed to WeakBrick with index:%i\n", index_Of_WeakBrick);
//		}
//		break;
//	case OBJECT_TYPE_WEAKBRICK:
//		{
//			obj = new CWeakBrick();
//			((CWeakBrick*)obj)->SetInitPosition(x, y);
//			WBrick.push_back((CWeakBrick*)obj);
//		//DebugOut(L"Weak Brick with index:%i has been pushed to list weakBricks\n", WeaKBrickIndex);
//		//WeaKBrickIndex++;
//		}
//		break;
//	case OBJECT_TYPE_SPECIALBRICK:
//		{
//			obj = new CSpecialBrick();
//			((CSpecialBrick*)obj)->SetInitPosition(x, y);
//			spBrick = (CSpecialBrick*)obj;
//		}
//		break;
//	case OBJECT_TYPE_COIN:
//		{	
//			obj = new CCoin();
//			((CCoin*)obj)->SetMario(player);
//			int index_Of_WeakBrick = atoi(tokens[4].c_str());
//			if (index_Of_WeakBrick != -1)
//			{
//				WBrick[index_Of_WeakBrick]->PushCoin((CCoin*)obj);
//				((CCoin*)obj)->isInsideWeakBrick = true;
//			}
//		}
//	break;
//	case OBJECT_TYPE_MYSTERYPIECE:
//	{
//		obj = new CMysteryPiece();
//		((CMysteryPiece*)obj)->SetMario(player);
//		if (x == 0)
//		{
//			pieces.push_back((CMysteryPiece*)obj);
//			((CMysteryPiece*)obj)->isFinished = true;
//		}
//		else
//		{
//			((CMysteryPiece*)obj)->SetState(MYSTERYPIECE_STATE_MUSHROOM);
//			lastpiece = (CMysteryPiece*)obj;
//		}
//
//	}
//	break;
//	case OBJECT_TYPE_MESSAGE:
//	{
//		obj = new CMessage();
//		lastpiece->SetMessage((CMessage*)obj);
//	}
//	break;
//	case OBJECT_TYPE_BOOMERANGBRO:
//	{
//		obj = new CBoomerangBro();
//		((CBoomerangBro*)obj)->SetInitPosition(x, y);
//		((CBoomerangBro*)obj)->SetMario(player);
//		bro = (CBoomerangBro*)obj;
//	}
//	break;
//	case OBJECT_TYPE_BOOMERANG:
//	{
//		obj = new CBoomerang();
//		((CBoomerang*)obj)->SetMario(player);
//		bro->SetBoomerang((CBoomerang*)obj);
//	}
//	break;
//	case OBJECT_TYPE_BOUNTYBUTTON:
//	{
//		obj = new CBountyButton();
//		((CBountyButton*)obj)->SetMario(player);
//		spBrick->SetBountyButton((CBountyButton*)obj);
//		((CBountyButton*)obj)->isInsideSpecialBrick = true;
//		for (unsigned int i = 0; i < WBrick.size(); i++)
//		{
//			((CBountyButton*)obj)->PushWeakBrick(WBrick[i]);
//		}
//	}
//	break;
//	case OBJECT_TYPE_BOUNTYBRICK:
//	{
//		obj = new CBountyBrick();
//		((CBountyBrick*)obj)->SetInitPosition(x, y);
//		((CBountyBrick*)obj)->SetMario(player);
//		int countdown = atoi(tokens[4].c_str());
//		((CBountyBrick*)obj)->SetCount(countdown);
//		BBrick.push_back((CBountyBrick*)obj);
//	}
//	break;
//	case OBJECT_TYPE_FIREFLOWER:
//	{
//		obj = new CFireFlower();
//		((CFireFlower*)obj)->SetMario(player);
//		((CFireFlower*)obj)->SetInitPosition(x, y);
//		flowers.push_back((CFireFlower*)obj);
//	}
//	break;
//	case OBJECT_TYPE_PIRANHA:
//	{
//		obj = new CPiranha();
//		((CPiranha*)obj)->SetMario(player);
//		((CPiranha*)obj)->SetInitPosition(x, y);
//	}
//	break;
//	case OBJECT_TYPE_FLOWER_FIREBALL:
//	{
//		obj = new CFireBallFlower();
//		((CFireBallFlower*)obj)->SetMario(player);
//		/*DebugOut(L"flower index: %i\n", FlowerIndex);
//		flowers[FlowerIndex]->SetFireBallFlower((CFireBallFlower*)obj);
//		DebugOut(L"flower fire ball was loaded\n");
//		FlowerIndex++;*/
//	}
//	break;
//	case OBJECT_TYPE_BOUNTY:
//	{
//		obj = new CBounty();
//		((CBounty*)obj)->SetMario(player);
//		/*BBrick[BountyBrickIndex]->SetBounty((CBounty*)obj);
//		if (CGame::GetInstance()->GetSceneNumber() == 2)
//		{
//			if (BountyBrickIndex == 3 || BountyBrickIndex == 5 || BountyBrickIndex == 7)
//			{
//				DebugOut(L"State of powerup%i\n", BOUNTY_STATE_POWERUP);
//				BBrick[BountyBrickIndex]->SetStateBounty(BOUNTY_STATE_POWERUP);
//				DebugOut(L"Bounty was added to bountybrick:%i with state:%i\n", BountyBrickIndex, obj->GetState());
//			}
//			else if (BountyBrickIndex == 8)
//			{
//				DebugOut(L"State of lifeup%i\n", BOUNTY_STATE_LIFEUP);
//				BBrick[BountyBrickIndex]->SetStateBounty(BOUNTY_STATE_LIFEUP);
//				DebugOut(L"Bounty was added to bountybrick:%i with state:%i\n", BountyBrickIndex, obj->GetState());
//			}
//			else
//				BBrick[BountyBrickIndex]->SetStateBounty(BOUNTY_STATE_COIN);
//
//		}
//		else if (CGame::GetInstance()->GetSceneNumber() == 3)
//		{
//			if (BountyBrickIndex == 0 || BountyBrickIndex == 3)
//			{
//				BBrick[BountyBrickIndex]->SetStateBounty(BOUNTY_STATE_LIFEUP);
//			}
//			else if (BountyBrickIndex == 1)
//			{
//				BBrick[BountyBrickIndex]->SetStateBounty(BOUNTY_STATE_POWERUP);
//			}
//			else
//				BBrick[BountyBrickIndex]->SetStateBounty(BOUNTY_STATE_COIN);
//		}
//		BBrick[BountyBrickIndex]->GetBounty()->SetInitPosition(x, y);
//		BountyBrickIndex++;*/
//
//	}
//	break;
//	case OBJECT_TYPE_FIREBALL:
//	{
//		obj = new CFireBall();
//		player->PushFireBall((CFireBall*)obj);
//	}
//	break;
//	case OBJECT_TYPE_TAIL:
//	{
//		obj = new CTail();
//		player->SetTail((CTail*)obj);
//	}
//	break;
//	case OBJECT_TYPE_KOOPAS:
//	{
//		int koopas_level = atoi(tokens[4].c_str());
//		obj = new CKoopas();
//		((CKoopas*)obj)->SetMario(player);
//		((CKoopas*)obj)->Setlevel(koopas_level);
//		((CKoopas*)obj)->SetInitPosition(x, y);
//	}
//	break;
//	case OBJECT_TYPE_HIDDENOBJECT:
//	{
//		int width = atoi(tokens[4].c_str());
//		int height = atoi(tokens[5].c_str());
//		obj = new CHiddenObject(x, y, width, height);
//	}
//	break;
//	case OBJECT_TYPE_HIDDENDOOR:
//	{
//		int width = atoi(tokens[4].c_str());
//		int height = atoi(tokens[5].c_str());
//		int upOrdown = atoi(tokens[6].c_str());
//		obj = new CHiddenDoor(x, y, width, height);
//		DebugOut(L"Hidden door was loaded\n");
//		((CHiddenDoor*)obj)->SetMario(player);
//		if (upOrdown == 0)
//			((CHiddenDoor*)obj)->UpOrDown = false;
//		else
//			((CHiddenDoor*)obj)->UpOrDown = true;
//	}
//	break;
//	case OBJECT_SYPE_AUTODOOR:
//	{
//		int width = atoi(tokens[4].c_str());
//		int height = atoi(tokens[5].c_str());
//		float des_x = (float)atof(tokens[6].c_str());
//		float des_y = (float)atof(tokens[7].c_str());
//		obj = new CAutoDoor(x, y, width, height, des_x, des_y);
//		DebugOut(L"Hidden door was loaded\n");
//		((CAutoDoor*)obj)->SetMario(player);
//	}
//	break;
//	case OBJECT_TYPE_PORTAL:
//	{
//		float r = (float)atof(tokens[4].c_str());
//		float b = (float)atof(tokens[5].c_str());
//		int scene_id = atoi(tokens[6].c_str());
//		obj = new CPortal(x, y, r, b, scene_id);
//		((CPortal*)obj)->SetMario(player);
//	}
//	break;
//	default:
//		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
//		return;
//	}
//	obj->SetType(object_type);						//set type for the object
//	// General object setup
//	obj->SetPosition(x, y);
//	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
//	obj->SetAnimationSet(ani_set);
//	}
//}
