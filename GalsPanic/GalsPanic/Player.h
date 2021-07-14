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

	// ��ġ
	Point pos;

	// ���� �̵� ����
	Point oldDir;

	// ũ��
	int size;

	// �ӷ�
	int speed;

	// �̵� ������ ����
	RECT rect;

	// ���� �������� ���ư��� �÷��̾��� ���
	vector<Point> path;

	// �밢�� �̵��� ����, ������ ������ ���� �̵��ϰ� �ϱ����� äũ ������ �߰��մϴ�.
	bool urBtn;
	bool ulBtn;
	bool drBtn;
	bool dlBtn;

public:

	Player(int _x, int _y, RECT _rect);

	// ���� �̵� ����
	Point GetOldDir() const { return oldDir; }
	void SetOldDir(Point _oldDir) { oldDir = _oldDir; }

	// �÷��̾� �׸���
	void DrawPlayer(Graphics *graphic);

	// �����̽� �� ������ �̵� : ���� ���������� �̵�
	void MoveWithSpace(int moveX, int moveY, vector<Point> &p);

	// �����̽� �� �ȴ����� �̵� : ���� ���������� �̵�
	void MoveWithoutSpace(int moveX, int moveY, vector<Point> &p);

	// �ڵ��ư���
	void MoveBack();

	// �밢���̵� (��� ����, ���� ������ �̵��ϴ°�)
	void MoveDiagonal(int moveX, int moveY, bool &btn);

	// ���� �̵�
	void MoveHorizontal(int moveX);

	// ���� �̵�
	void MoveVertical(int moveY);
};

#endif // !__PLAYER_H__