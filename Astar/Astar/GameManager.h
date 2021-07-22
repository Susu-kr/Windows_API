#pragma once
#include "framework.h"
struct INT_POS
{
	int x;
	int y;
};
class Node
{
public:
	bool	isWall;
	vector<int> ParentNode;

	int		x, y;
	// G : ���ۺ��� �̵��Ÿ�
	// H : |����| + |����| ��ֹ� �����Ͽ� ��ǥ������ �Ÿ�
	// F : G + H
	int		G, H, F;

	Node() { isWall = false, x = 0, y = 0; }
	Node(bool _isWall, int _x, int _y)
	{
		isWall = _isWall; x = _x; y = _y;
	}
	bool operator == (const Node& other) {
		if (x == other.x && y == other.y) return true;
		return false;
	}
	bool operator != (const Node& other) {
		if (x != other.x || y != other.y) return true;
		return false;
	}
};



class GameManager
{
public:
	vector<pair<int, int>> wall;
	// bottomLeft, topRight : �����ڸ� ����
	// startPos, targetPos : ������ǥ, ������ǥ
	INT_POS bottomRight, topLeft, startPos, targetPos;
	// ����� ����
	int sizeX, sizeY;
	// ��ü���� MAP
	Node** NodeArray = nullptr;
	// ���ۺ��, �������, ������
	Node StartNode, TargetNode, CurNode;
	vector<Node> OpenList, CloseList, FinalNodelist, CheckList;
	GameManager();
	~GameManager();
	void Init(RECT r);
	void Reset(RECT r);
	void Delete_Map();
	void Draw_Map(HWND hWnd, RECT rect, HDC hdc);
	void CreateBlock(int x, int y, bool d_flag);
	void Start_to_Target(int x, int y, int state);
	void PathFinding();
	void OpenListAdd(int c_X, int c_Y);
	void Draw_Path();
	int GetDistance(int sx, int sy, int dx, int dy);
};