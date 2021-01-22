#pragma once
#include "Node.h"
#include "WorldScence.h"
CNode::CNode(int id, int l, int r, int t, int b, int type)
{
	node_id = id;
	left = l;
	right = r;
	top = t;
	bottom = b;
	typeNode = type;
}
CNode::~CNode()
{

}
CNode* CNode::FindNote(int direction)
{
	vector<CNode*> Nodes = ((CWorldScence*)CGame::GetInstance()->GetCurrentScene())->GetNodes();
	switch (direction)
	{
	case 1:
		for (unsigned int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->y == this->y - 50 && Nodes[i]->x == this->x)
				return Nodes[i];
		}
		break;
	case 2:
		for (unsigned int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->y == this->y + 50 && Nodes[i]->x == this->x)
				return Nodes[i];
		}
		break;
	case 3:
		for (unsigned int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->y == this->y  && Nodes[i]->x == this->x+50)
				return Nodes[i];
		}
		break;
	case 4:
		for (unsigned int i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->y == this->y && Nodes[i]->x == this->x-50)
				return Nodes[i];
		}
		break;	
	}
	return NULL;
}