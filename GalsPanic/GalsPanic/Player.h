#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "framework.h"

#include <vector>

using namespace Gdiplus;
using std::vector;

class Player
{
private:

	// 위치
	Point pos;

	// 이전 이동 방향
	Point oldDir;

	// 크기
	int size;

	// 속력
	int speed;

	// 이동 가능한 영역
	RECT rect;

	// 닫힌 공간으로 나아갈때 플레이어의 경로
	vector<Point> path;

	// 대각선 이동시 수직, 수평을 번갈아 가며 이동하게 하기위해 채크 변수를 추가합니다.
	bool urBtn;
	bool ulBtn;
	bool drBtn;
	bool dlBtn;

public:

	Player(int _x, int _y, RECT _rect);

	// 이전 이동 방향
	Point GetOldDir() const { return oldDir; }
	void SetOldDir(Point _oldDir) { oldDir = _oldDir; }

	// 플레이어 그리기
	void DrawPlayer(Graphics *graphic);

	// 스페이스 바 누르고 이동 : 닫힌 영역에서의 이동
	void MoveWithSpace(int moveX, int moveY, vector<Point> &p);

	// 스페이스 바 안누르고 이동 : 열린 영역에서의 이동
	void MoveWithoutSpace(int moveX, int moveY, vector<Point> &p);

	// 뒤돌아가기
	void MoveBack();

	// 대각선이동 (사실 수평, 수직 번갈아 이동하는것)
	void MoveDiagonal(int moveX, int moveY, bool &btn);

	// 수평 이동
	void MoveHorizontal(int moveX);

	// 수직 이동
	void MoveVertical(int moveY);
};

#endif // !__PLAYER_H__