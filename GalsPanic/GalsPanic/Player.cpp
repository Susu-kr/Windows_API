#define _CRT_SECURE_NO_WARNINGS

#include "Player.h"
#include "Util.h"

Player::Player(int _x, int _y, RECT _rect)
	: pos({ _x, _y }), oldDir({ 0, 0 }), size(8), speed(4), rect({ _rect.left + size / 2, _rect.top + size / 2, _rect.right - size / 2, _rect.bottom - size / 2 })
{
	urBtn = ulBtn = drBtn = dlBtn = false;
}

void Player::DrawPlayer(Graphics * graphic)
{

	SolidBrush brush(Color(255, 255, 0, 0));
	graphic->FillRectangle(&brush, pos.X - size / 2, pos.Y - size / 2, size, size);

	int pathSize = (int)path.size();

	Pen pen(Color(255, 100, 200, 150));

	// 플레이어가 지나온 경로 그리기
	for (int i = 0; i < pathSize - 1; ++i)
		DrawLine(graphic, path[i], path[i + 1]);

	// 마지막 지점과 플레이어 사이의 선분
	if (pathSize > 0)
		DrawLine(graphic, path[path.size() - 1], pos);
}

void Player::MoveWithSpace(int moveX, int moveY, vector<Point>& p)
{
	// 범위에 벗어나는 경우 이동 불가
	if (!InRect(pos.X, pos.Y + moveY * speed, rect))
		moveY = 0;

	if (!InRect(pos.X + moveX * speed, pos.Y, rect))
		moveX = 0;

	Point oldPos = pos;

	if (moveX == 1 && moveY == -1)
		MoveDiagonal(moveX, moveY, urBtn);
	else if (moveX == -1 && moveY == -1)
		MoveDiagonal(moveX, moveY, ulBtn);
	else if (moveX == 1 && moveY == 1)
		MoveDiagonal(moveX, moveY, drBtn);
	else if (moveX == -1 && moveY == 1)
		MoveDiagonal(moveX, moveY, dlBtn);
	else if (moveX == 0 && moveY == -1)
		MoveVertical(moveY);
	else if (moveX == 0 && moveY == 1)
		MoveVertical(moveY);
	else if (moveX == 1 && moveY == 0)
		MoveHorizontal(moveX);
	else if (moveX == -1 && moveY == 0)
		MoveHorizontal(moveX);

	// 중간에 열린 영역이 끼어있을수도 있기 때문에 가운데 영역도 확인
	Point m = { (pos.X + oldPos.X) / 2, (pos.Y + oldPos.Y) / 2 };

	// 열린 영역 안에있는지 확인, 가장자리는 체크 안함
	if (InPolygon(pos, p, false) || InPolygon(m, p, false))
	{
		oldDir = { 0, 0 };
		pos = oldPos;
		path.clear();
		return;
	}

	// 경로 점 추가를 위해 방향을 비교하고, 갱신한다.
	// 이전 진행방향과 다음 진행방향이 다르면 새로운 점을 경로에 추가한다

	Point newPos = pos;

	Point newDir = newPos - oldPos;
	Point zero = { 0, 0 };

	if (newDir != zero && newDir != oldDir)
		path.push_back(oldPos);

	if (newDir != zero)
		oldDir = newDir;

	// 도착지점이 열린공간의 가장자리일 경우
	if (OnCircuit(pos, p))
	{
		oldDir = { 0, 0 };

		if (path.empty())
		{
			path.clear();
		}
		else
		{
			path.push_back(pos);
			CombinePolygon(p, path);
		}
	}
}

void Player::MoveWithoutSpace(int moveX, int moveY, vector<Point>& p)
{
	if (path.empty())
	{
		Point next = pos + Point(moveX * speed, moveY * speed);

		bool checkMove = false;

		if (moveY != 0 && OnCircuit(pos.X, next.Y, p))
		{
			Point m = { pos.X, (pos.Y + next.Y) / 2 };

			if (!InPolygon(m, p, false) && OnCircuit(m, p))
			{
				checkMove = true;
				pos.Y = next.Y;
			}
		}

		if (!checkMove && moveX != 0 && OnCircuit(next.X, pos.Y, p))
		{
			Point m = { (pos.X + next.X) / 2, pos.Y };

			if (!InPolygon(m, p, false) && OnCircuit(m, p))
				pos.X = next.X;
		}
	}
}

void Player::MoveBack()
{
	if (path.empty()) return;

	if (pos == path.back())
		path.pop_back();

	if (path.empty()) return;

	Point backP = path.back();

	Point backDir = GetDir(backP - pos);
	Point dir = backDir * speed;

	pos = pos + dir;
	oldDir = -1 * backDir;
}

void Player::MoveDiagonal(int moveX, int moveY, bool & btn)
{
	int nextX = pos.X + speed * moveX;
	int nextY = pos.Y + speed * moveY;

	if (btn)
	{
		if (!OnPath(nextX, pos.Y, path) && (path.empty() || !OnLine(nextX, pos.Y, path.back(), pos)))
			pos.X = nextX;
		else if (!OnPath(pos.X, nextY, path) && (path.empty() || !OnLine(pos.X, nextY, path.back(), pos)))
			pos.Y = nextY;
		else return;

		btn = false;
	}
	else
	{
		if (!OnPath(pos.X, nextY, path) && (path.empty() || !OnLine(pos.X, nextY, path.back(), pos)))
			pos.Y = nextY;
		else if (!OnPath(nextX, pos.Y, path) && (path.empty() || !OnLine(nextX, pos.Y, path.back(), pos)))
			pos.X = nextX;
		else return;

		btn = true;
	}
}

void Player::MoveHorizontal(int moveX)
{
	int nextX = pos.X + speed * moveX;

	if (!OnPath(nextX, pos.Y, path) && (path.empty() || !OnLine(nextX, pos.Y, path.back(), pos)))
		pos.X = nextX;
}

void Player::MoveVertical(int moveY)
{
	int nextY = pos.Y + speed * moveY;

	if (!OnPath(pos.X, nextY, path) && (path.empty() || !OnLine(pos.X, nextY, path.back(), pos)))
		pos.Y = nextY;
}