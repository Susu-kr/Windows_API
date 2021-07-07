#pragma once
#include "framework.h"
#include "DrawImage.h"
#include "GDIPLUS.h"
#include <commdlg.h>
#pragma comment(lib, "msimg32.lib")

// image
HBITMAP				hBackImage;
BITMAP				bitBack;
HBITMAP				hBackImage2;
BITMAP				bitBack2;

bool				sizeChangeFlag = false;
int					flag;

HBITMAP				hAniImage;
BITMAP				bitAni;
const int			SPRITE_SIZE_X = 32;
const int			SPRITE_SIZE_Y = 32;
POINT				AniPos;
int					Start_Pos_X = 0;
int					Start_Pos_Y = 0;
// run
int					RUN_FRAME_MAX = 0;
int					RUN_FRAME_MIN = 0;
int					curframe = RUN_FRAME_MIN;
// Text
RECT				rectView;
void				DrawRectText(HDC hdc);
void				Update();
void				UpdateFrame();
// double buffering
HBITMAP				hDoubleBufferImage;
HBITMAP				hReverseImage;
void				DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);
// 

void				CreateBitmap();
void				DeleteBitmap();
void				UpdateFrame(HWND hWnd);
VOID CALLBACK		AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
// <<

void Update() {
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;
	if (newTime - oldTime < 50) return;
	oldTime = newTime;

	// : to do something
	UpdateFrame();
}

void UpdateFrame() {
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		AniPos.x += 5;
		flag = 6;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		AniPos.x -= 5;
		flag = 4;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		AniPos.y -= 5;
		flag = 8;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		AniPos.y += 5;
		flag = 2;
	}
}

void DrawRectText(HDC hdc)
{
	static int yPos = 0;
	SetTextColor(hdc, RGB(255, 0, 0));
	TCHAR strTest[] = _T("달성률 : ");
	TextOut(hdc, 30, 0, strTest, _tcslen(strTest));
}

void CreateBitmap()
{
	// >> : back image
	{
		hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/수지1.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBackImage, sizeof(BITMAP), &bitBack);
	}
	{
		hBackImage2 = (HBITMAP)LoadImage(NULL, TEXT("images/수지2.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBackImage2, sizeof(BITMAP), &bitBack2);
	}
	
	hAniImage = (HBITMAP)LoadImage(NULL, TEXT("images/8D.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hAniImage, sizeof(BITMAP), &bitAni);

	RUN_FRAME_MAX = bitAni.bmWidth / SPRITE_SIZE_X / 2 - 1;
	RUN_FRAME_MIN = 0;
	curframe = RUN_FRAME_MIN;
	AniPos = { 200 ,400 };
	// << 
}


void DeleteBitmap()
{
	DeleteObject(hBackImage);
	DeleteObject(hBackImage2);
	DeleteObject(hAniImage);
}


VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	curframe++;
	if (curframe > RUN_FRAME_MAX)
		curframe = RUN_FRAME_MIN;
	InvalidateRect(hWnd, NULL, false);
}

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	int bx, by;

	HDC hMemDC2;
	HBITMAP hOldBitmap2;

	hMemDC = CreateCompatibleDC(hdc);
	if (!hMemDC) {
		MessageBox(hWnd, _T("CreateCompatibleDC failed"),
			_T("Error"), MB_OK);
		return;
	}
	if (hDoubleBufferImage == NULL) {
		hDoubleBufferImage = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
	}

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);
	// >> : back
	{
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage);
		bx = bitBack.bmWidth;
		by = bitBack.bmHeight;

		BitBlt(hMemDC, 0, 0, bx, by, hMemDC2, 0, 0, SRCCOPY);
		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}
	//{
	//	hMemDC2 = CreateCompatibleDC(hMemDC);
	//	hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage2);
	//	bx = bitBack2.bmWidth;
	//	by = bitBack2.bmHeight;
	//	TransparentBlt(hMemDC, 0, 0, bx, by, hMemDC2,
	//		0, 0, bx, by, RGB(255, 0, 255));
	//	SelectObject(hMemDC2, hOldBitmap2);
	//	DeleteDC(hMemDC2);
	//}
	Gdi_Draw(hMemDC);

	{
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hAniImage);

		HDC hReverseMemDC;
		HBITMAP hOldBitmap3;
		hReverseMemDC = CreateCompatibleDC(hMemDC);
		if (!hReverseImage) {
			hReverseImage = CreateCompatibleBitmap(hMemDC2, bx, by);
		}
		hOldBitmap3 = (HBITMAP)SelectObject(hReverseMemDC, hReverseImage);

		bx = SPRITE_SIZE_X;
		by = SPRITE_SIZE_Y;
		switch (flag)
		{
		case 2:
			Start_Pos_X = curframe * SPRITE_SIZE_X;
			Start_Pos_Y = 0 * SPRITE_SIZE_Y;
			break;
		case 4:
			Start_Pos_X = curframe * SPRITE_SIZE_X;
			Start_Pos_Y = 1 * SPRITE_SIZE_Y;
			break;
		case 6:
			Start_Pos_X = curframe * SPRITE_SIZE_X;
			Start_Pos_Y = 2 * SPRITE_SIZE_Y;
			break;
		case 8:
			Start_Pos_X = curframe * SPRITE_SIZE_X;
			Start_Pos_Y = 3 * SPRITE_SIZE_Y;
			break;
		default:
			break;
		}
		TransparentBlt(hMemDC, AniPos.x, AniPos.y, bx, by, hMemDC2,
			Start_Pos_X, Start_Pos_Y, bx, by, RGB(255, 255, 255));

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}

	BitBlt(hdc, 0, 0, rectView.right, rectView.bottom,
		hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}
