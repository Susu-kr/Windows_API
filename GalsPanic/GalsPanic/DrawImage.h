#pragma once
#include "framework.h"
#include <commdlg.h>
#pragma comment(lib, "msimg32.lib")

// image
HBITMAP				hBackImage;
BITMAP				bitBack;
HBITMAP				hBackImage2;
BITMAP				bitBack2;

bool				sizeChangeFlag = false;
int					flag;
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
	//if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
	//	AniPos.x += 5;
	//	flag = 6;
	//}
	//if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
	//	AniPos.x -= 5;
	//	flag = 4;
	//}
	//if (GetAsyncKeyState(VK_UP) & 0x8000) {
	//	AniPos.y -= 5;
	//	flag = 8;
	//}
	//if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
	//	AniPos.y += 5;
	//	flag = 2;
	//}
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
	// << 
}


void DeleteBitmap()
{
	DeleteObject(hBackImage);
	DeleteObject(hBackImage2);
}


VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	/*curframe++;
	if (curframe > RUN_FRAME_MAX)
		curframe = RUN_FRAME_MIN;
	InvalidateRect(hWnd, NULL, false);*/
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
	{
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage2);
		bx = bitBack2.bmWidth;
		by = bitBack2.bmHeight;

		//HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255,0,255));
		//HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC2, myBrush);

		//Polygon(hMemDC2, vertexPool, polyVertexCont.size());

		TransparentBlt(hMemDC, 0, 0, bx, by, hMemDC2,
			0, 0, bx, by, RGB(255, 0, 255));
		//SelectObject(hMemDC2, oldBrush);
		//DeleteObject(myBrush);

		//player.show(hMemDC, moveT);

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}
	//Gdi_Draw(hMemDC);

	BitBlt(hdc, 0, 0, rectView.right, rectView.bottom,
		hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}
