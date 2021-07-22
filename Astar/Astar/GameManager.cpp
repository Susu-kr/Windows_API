#include "GameManager.h"

/*
	OPEN에 시작 넣기
	CLOSED

	While
	{
		Cur = OPEN리스트 중 가장작은 F (F가 같다면 H가 작은)
		OPEN에서 Cur 제거
		CLOSE에 Cur 넣기

		if(Cur == Target) 끝

		4방향검사 (대각선 허용시 8번)
		{
			if(범위 안 && 벽이 아님 && CLOSED에 없다)
			{
				대각선 허용시 벽 사이로 통과 x
				코너를 가로질러 가지 않을 시 이동 중에 수직 수평 장애물 x
			}

			이동비용이 이웃노드 G보다 작거나 || OPEN에 이웃노드가 없다면
			G, H, 부모 설정 후 OPEN에 추가
		}
	}
	// https://goraniunity2d.blogspot.com/2019/10/2d-140.html
*/

GameManager::GameManager()
{
}

GameManager::~GameManager()
{

}

void GameManager::Init(RECT R)
{
	// NodeArray의 크기 설정, isWall, x, y 대입

	sizeX = (R.right - R.left)/ 30 + 1;
	sizeY = (R.bottom - R.top) / 30 + 1;
	topLeft = { 0, 0 };
	bottomRight = { sizeY, sizeX };
	NodeArray = new Node*[sizeY];
	for (int i = 0; i < sizeY; i++)
		NodeArray[i] = new Node[sizeX];

	int i = 0, j = 0;
	int y_pos = 15;
	for (int i = 0; i < sizeY; i++) {
		int x_pos = 15;
		for (int j = 0; j < sizeX; j++) {
			NodeArray[i][j].isWall = false;
			NodeArray[i][j].x = x_pos;
			NodeArray[i][j].y = y_pos;
			x_pos += 30;
		}
		y_pos += 30;
	}
}

void GameManager::Reset(RECT R)
{
	wall.clear();
	topLeft = { 0, 0 };
	bottomRight = { 0, 0 };
	startPos = { 0, 0 };
	targetPos = { 0, 0 };
	sizeX = 0; sizeY = 0;
	Delete_Map();
	OpenList.clear(); CloseList.clear(), FinalNodelist.clear(), CheckList.clear();
	Init(R);
}

void GameManager::Delete_Map()
{
	for (int i = 0; i < sizeY; i++)
		delete NodeArray[i];
	delete NodeArray;
}

void GameManager::Draw_Map(HWND hWnd, RECT rect, HDC hdc)
{
	HDC	hdcBuff;
	HBITMAP MemDC, OldDC;
	hdc = GetDC(hWnd);
	hdcBuff = CreateCompatibleDC(hdc);
	MemDC = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	OldDC = (HBITMAP)SelectObject(hdcBuff, MemDC);
	PatBlt(hdcBuff, 0, 0, rect.right, rect.bottom, WHITENESS);

	HBRUSH Brush, oldBrush;
	int y_pos = 15;
	for (int i = 0; i < sizeY - 1; i++) {
		int x_pos = 15;
		for (int j = 0; j < sizeX - 1; j++) {
			if(!NodeArray[i][j].isWall)
				Rectangle(hdcBuff, x_pos - 15, y_pos - 15, x_pos + 15, y_pos + 15);
			else {
				Brush = CreateSolidBrush(RGB(195, 195, 195));
				oldBrush = (HBRUSH)SelectObject(hdcBuff, Brush);
				Rectangle(hdcBuff, x_pos - 15, y_pos - 15, x_pos + 15, y_pos + 15);
				SelectObject(hdcBuff, oldBrush);
				DeleteObject(Brush);
			}
			if (NodeArray[i][j].x == startPos.x && NodeArray[i][j].y == startPos.y
				|| NodeArray[i][j].x == targetPos.x && NodeArray[i][j].y == targetPos.y	)
			{
				Brush = CreateSolidBrush(RGB(255, 242, 0));
				oldBrush = (HBRUSH)SelectObject(hdcBuff, Brush);
				Rectangle(hdcBuff, x_pos - 15, y_pos - 15, x_pos + 15, y_pos + 15);
				SelectObject(hdcBuff, oldBrush);
				DeleteObject(Brush);
			}
			x_pos += 30;
		}
		y_pos += 30;
	}

	// OpenList 색 변경
	Brush = CreateSolidBrush(RGB(196, 255, 14));
	oldBrush = (HBRUSH)SelectObject(hdcBuff, Brush);
	for (int i = 0; i < CheckList.size(); i++)
	{
		Rectangle(hdcBuff,
			NodeArray[CheckList[i].y][CheckList[i].x].x - 15,
			NodeArray[CheckList[i].y][CheckList[i].x].y - 15,
			NodeArray[CheckList[i].y][CheckList[i].x].x + 15,
			NodeArray[CheckList[i].y][CheckList[i].x].y + 15);
	}
	SelectObject(hdcBuff, oldBrush);
	DeleteObject(Brush);
	// CloseList 색 변경
	Brush = CreateSolidBrush(RGB(0, 168, 243));
	oldBrush = (HBRUSH)SelectObject(hdcBuff, Brush);
	for (int i = 0; i < CloseList.size(); i++)
	{
		Rectangle(hdcBuff,
			NodeArray[CloseList[i].y][CloseList[i].x].x - 15,
			NodeArray[CloseList[i].y][CloseList[i].x].y - 15,
			NodeArray[CloseList[i].y][CloseList[i].x].x + 15,
			NodeArray[CloseList[i].y][CloseList[i].x].y + 15);
	}
	SelectObject(hdcBuff, oldBrush);
	DeleteObject(Brush);

	// FinalNodelist 색 변경
	Brush = CreateSolidBrush(RGB(236, 28, 36));
	oldBrush = (HBRUSH)SelectObject(hdcBuff, Brush);
	for (int i = FinalNodelist.size() -1; i >= 0; i--)
	{
		Rectangle(hdcBuff,
			NodeArray[FinalNodelist[i].y][FinalNodelist[i].x].x - 15,
			NodeArray[FinalNodelist[i].y][FinalNodelist[i].x].y - 15,
			NodeArray[FinalNodelist[i].y][FinalNodelist[i].x].x + 15,
			NodeArray[FinalNodelist[i].y][FinalNodelist[i].x].y + 15);
	}
	SelectObject(hdcBuff, oldBrush);
	DeleteObject(Brush);

	BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcBuff, 0, 0, SRCCOPY);
	SelectObject(hdcBuff, OldDC);
	DeleteObject(MemDC);
	DeleteDC(hdcBuff);
	ReleaseDC(hWnd, hdc);

	// https://3001ssw.tistory.com/43
}

void GameManager::CreateBlock(int x, int y, bool d_flag)
{
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			if (NodeArray[i][j].x - 15 < x && NodeArray[i][j].x + 15 > x
				&& NodeArray[i][j].y - 15 < y && NodeArray[i][j].y + 15 > y)
			{
				if (!NodeArray[i][j].isWall)
					if (!d_flag)
						NodeArray[i][j].isWall = true;
				else 
					if (d_flag) 
						NodeArray[i][j].isWall = false;
				return;
			}
		}
	}
}

void GameManager::Start_to_Target(int x, int y, int state)
{
	for (int i = 0; i < sizeY; i++) {
		for (int j = 0; j < sizeX; j++) {
			if (NodeArray[i][j].x - 15 < x && NodeArray[i][j].x + 15 > x
				&& NodeArray[i][j].y - 15 < y && NodeArray[i][j].y + 15 > y)
				switch (state)
				{
				case 1:
					{
						startPos = { NodeArray[i][j].x, NodeArray[i][j].y };
						StartNode.x = j;
						StartNode.y = i;
						return;
					}
				case 2:
					{
						targetPos = { NodeArray[i][j].x, NodeArray[i][j].y };
						TargetNode.x = j;
						TargetNode.y = i;
						PathFinding();
						return;
					}
				}
		}
	}
}

void GameManager::PathFinding()
{
	StartNode.G = 0;
	StartNode.H = GetDistance(StartNode.x, StartNode.y, TargetNode.x, TargetNode.y);
	StartNode.F = StartNode.G + StartNode.H;
	OpenList.push_back(StartNode);

	while (OpenList.size() > 0)
	{
		// OpenList 중 가장 F가 작고 F가 같다면 H가 작은 걸 현재노드로 하고 Open -> Close로 옮기기
		CurNode = OpenList[0];
		int cnt = 0;
		for (int i = 1; i < OpenList.size(); i++) {
			if (OpenList[i].F <= CurNode.F)
			{
				CurNode = OpenList[i];
				cnt = i;
			}
		}
		OpenList.erase(OpenList.begin() + cnt);
		CloseList.push_back(CurNode);

		// 마지막
		if (CurNode == TargetNode)
		{
			Node TargetCurNode = CurNode;
			while (TargetCurNode != StartNode)
			{
				FinalNodelist.push_back(TargetCurNode);
				for (int i = CloseList.size() - 1; i >= 0; i--) {
					if (CloseList[i].x == TargetCurNode.ParentNode.x && CloseList[i].y == TargetCurNode.ParentNode.y)
					{
						TargetCurNode = CloseList[i];
						break;
					}
				}
			}
			FinalNodelist.push_back(StartNode);

			return;
		}

		// ↗↖↙↘
		OpenListAdd(CurNode.x + 1, CurNode.y - 1);
		OpenListAdd(CurNode.x - 1, CurNode.y - 1);
		OpenListAdd(CurNode.x - 1, CurNode.y + 1);
		OpenListAdd(CurNode.x + 1, CurNode.y + 1);
		// ↑ → ↓ ←
		OpenListAdd(CurNode.x, CurNode.y - 1);
		OpenListAdd(CurNode.x + 1, CurNode.y);
		OpenListAdd(CurNode.x, CurNode.y + 1);
		OpenListAdd(CurNode.x - 1, CurNode.y);
	}
}

void GameManager::OpenListAdd(int c_X, int c_Y)
{
	// 상하좌우 범위를 벗어나지 않고, 벽이 아니면서
	if (c_X >= 0 && c_X < bottomRight.x + 1 && c_Y >= 0 && c_Y < bottomRight.y + 1
		&& !NodeArray[c_Y][c_X].isWall)
	{
		// CloseList에 없다면
		for (int i = 0; i < CloseList.size(); i++) {
			if (CloseList[i].x == c_X && CloseList[i].y == c_Y) return;
		}	
		//// 벽 사이로 통과가 안됌
		//if (NodeArray[CurNode.y - topLeft.y][c_X - topLeft.x].isWall
		//	&& NodeArray[c_Y - topLeft.y][CurNode.x - topLeft.x].isWall) return;

		// 이웃노드에 넣고, 상하좌우 = 10, 대각선 = 14
		Node N_Node(false, c_X, c_Y);
		int MoveCost = CurNode.G + (CurNode.x - c_X == 0 || CurNode.y - c_Y == 0 ? 10 : 14);
		int NextH = GetDistance(N_Node.x, N_Node.y, TargetNode.x, TargetNode.y);
		// 이동비용이 이웃노드 G 보다 작거나 또는 OpenList에 이웃노드가 없다면 G, H, ParentNode를 설정 후 OpenList에 추가
		for (int i = 0; i < OpenList.size(); i++) {
			if (OpenList[i] == N_Node && OpenList[i].F > MoveCost + NextH) {
				OpenList[i].G = MoveCost;
				OpenList[i].H = NextH;
				OpenList[i].F = OpenList[i].G + OpenList[i].H;
				OpenList[i].ParentNode = { CurNode.x, CurNode.y };
				return;
			}
		}
		N_Node.G = MoveCost;
		N_Node.H = NextH;
		N_Node.F = N_Node.G + N_Node.H;
		N_Node.ParentNode = { CurNode.x, CurNode.y };
		OpenList.push_back(N_Node);
		CheckList.push_back(N_Node);
	}
}

void GameManager::Draw_Path()
{
}

int GameManager::GetDistance(int sx, int sy, int dx, int dy)
{
	int absX = abs(sx - dx);
	int absY = abs(sy - dy);

	int minV, maxV;

	if (absX < absY)
	{
		minV = absX;
		maxV = absY;
	}
	else
	{
		minV = absY;
		maxV = absX;
	}

	return minV * 14 + (maxV - minV) * 10;
}
