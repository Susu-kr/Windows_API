#pragma once

#include "framework.h"
#include "Sound.h"
enum STATE { START, INGAME, CLEAR, OVER, END };
enum DIFF { EASY, NORMAL, HARD };
class GameManager
{
	UINT nFlags;
	ULONG_PTR			g_GdiToken;
	int					state;
	int					difficulty;
	CSound				sound;
	HBITMAP				hDoubleBufferImage;
	bool				chk_op;
	bool				chk_game;
	bool				chk_ability;
	bool				passive[4]; // 0 : 없음 , 1 : 응급 처치, 2 : 생존 키트, 3 : 운 좋은 놈
	bool				active[4]; // 0 : 없음, 1 : 운이 좌우하는 게임, 2 : 엉망진창 3 : 고소공포증
	string				p_str[3];
	string				a_str[3];

public:
	RECT				screenRect;
	void				Init();
	void				GameReset();
	void				Update();
	void				DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
	void				Transition(int _state);
	void				GDI_Init();
	void				GDI_Draw(HDC hdc);
	void				GDI_End();


	bool				Click(float x, float y);


	int getState() { return state; }
	int getDifficulty() { return difficulty; }
	void DrawStartScreen(Graphics& graphics);
	void DrawGameScreen(Graphics& graphics);
	void DrawClearScreen(Graphics& graphics);
	void DrawOverScreen(Graphics& graphics);
	void DrawEndScreen(Graphics& graphics);

};
