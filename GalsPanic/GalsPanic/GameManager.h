#pragma once

#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "framework.h"

#include "InputManager.h"
#include "Player.h"

#include <vector>

using namespace Gdiplus;
using std::vector;

class GameManager
{
private:

	// �Է� ����
	InputManager* input;

	// ��� �̹���
	Image *image;

	// �÷��̾�
	Player *player;

	// ��ũ�� ũ��
	int screenW, screenH;

	// ��� �̹��� ũ��
	int imageW, imageH;

	// ���� ���� ����
	vector<Point> opened;

	// ���� ���� �׸���
	void DrawClosed(Graphics *graphic);

	// ���� ���� �׸���
	void DrawOpened(Graphics *graphic);

	// ���� ȭ�� �׸���
	void DrawGame(Graphics *graphic);

public:
	GameManager();
	~GameManager();

	// �Է� ��ü ��ȯ
	InputManager* Input() const { return input; }

	// �ʱ�ȭ
	void Init();

	// ������Ʈ
	void Update();

	// �׸���
	void Draw(HWND hWnd, HDC hdc);
};



#endif // !__GAMEMANAGER_H__