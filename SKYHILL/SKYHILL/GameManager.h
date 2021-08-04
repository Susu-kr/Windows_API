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
	bool				passive[4]; // 0 : ���� , 1 : ���� óġ, 2 : ���� ŰƮ, 3 : �� ���� ��
	bool				active[4]; // 0 : ����, 1 : ���� �¿��ϴ� ����, 2 : ������â 3 : ��Ұ�����
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
