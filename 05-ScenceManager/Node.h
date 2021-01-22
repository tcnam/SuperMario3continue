#pragma once
#include "GameObject.h"
#include <vector>

class CNode :public CGameObject
{
	int typeNode;
	int left, top, right, bottom;
	int node_id;
public:
	CNode(int id, int l, int r, int t, int b, int ctype);
	~CNode();
	virtual void Render() {};
	virtual void GetBoundingBox(float & left, float& top, float & right, float& bottom){}
	int GetType(){return typeNode;}
	void SetTypeNode(int n){type = n;}
	int GetNodeId() { return node_id; }
	int GetTop() { return top; }
	int GetBottom() { return bottom; }
	int GetRight() { return right; }
	int GetLeft() { return left; }
	void SetTop(int t) { top = t; }
	void SetBottom(int b) { bottom = b; }
	void SetRight(int r) { right = r; }
	void SetLeft(int l) { left = l; }
	CNode* FindNote(int direction);
};