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

	// 입력 관리
	InputManager* input;

	// 배경 이미지
	Image *image;

	// 플레이어
	Player *player;

	// 스크린 크기
	int screenW, screenH;

	// 배경 이미지 크기
	int imageW, imageH;

	// 열린 공간 벡터
	vector<Point> opened;

	// 닫힌 공간 그리기
	void DrawClosed(Graphics *graphic);

	// 열린 공간 그리기
	void DrawOpened(Graphics *graphic);

	// 게임 화면 그리기
	void DrawGame(Graphics *graphic);

public:
	GameManager();
	~GameManager();

	// 입력 객체 반환
	InputManager* Input() const { return input; }

	// 초기화
	void Init();

	// 업데이트
	void Update();

	// 그리기
	void Draw(HWND hWnd, HDC hdc);
};



#endif // !__GAMEMANAGER_H__