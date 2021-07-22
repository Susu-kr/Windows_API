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
	// G : 시작부터 이동거리
	// H : |가로| + |세로| 장애물 무시하여 목표까지의 거리
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
	// bottomLeft, topRight : 가장자리 범위
	// startPos, targetPos : 시작좌표, 도착좌표
	INT_POS bottomRight, topLeft, startPos, targetPos;
	// 블록의 갯수
	int sizeX, sizeY;
	// 전체적인 MAP
	Node** NodeArray = nullptr;
	// 시작블록, 도착블록, 현재블록
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