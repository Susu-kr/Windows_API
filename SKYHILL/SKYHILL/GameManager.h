#pragma once

#include "framework.h"
#include "Sound.h"
enum STATE { START, INGAME, CLEAR, OVER, END };

class GameManager
{
	UINT nFlags;
	ULONG_PTR			g_GdiToken;
	int					state;
	CSound*				sound;
	HBITMAP				hDoubleBufferImage;

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

	int getState() { return state; }
	void DrawStartScreen(Graphics& graphics);
	void DrawGameScreen(Graphics& graphics);
	void DrawClearScreen(Graphics& graphics);
	void DrawOverScreen(Graphics& graphics);
	void DrawEndScreen(Graphics& graphics);
};
