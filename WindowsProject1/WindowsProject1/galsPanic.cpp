// galsPanic.cpp : Defines the entry point for the application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "WindowsProject1.h"
#include "movingPoint.h"
//#include <commdlg.h>

#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100
#define windowWidth 800
#define windowHeight 800

using namespace std;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


HBITMAP hBackImage, hBackImage2;
BITMAP bitBack, bitBack2;
HBITMAP hTransparentImage;
BITMAP bitTransparent;
HBITMAP hAniImage;
BITMAP bitAni;

bool sizeChangeFlag = 0;

TCHAR sKeyState[128];
vector<POINT> polyVertexCont;
POINT vertexPool[1360];
POINT moveT, tmp;

vector<polyLine> polyLineCont;

movingPoint player;
POINT tmpVec = { 0, 0 };
int movState = -1;

LONG polyArea = 0;

RECT rectView;
RECT rectView2;
HBITMAP hDoubleBufferImage;

void DrawRectText(HDC);
void DrawBitmapDoubleBuffering(HWND, HDC);

void CreateBitmap();
void DrawBitmap(HWND, HDC);
void DeleteBitmap();
VOID CALLBACK KeyStateProc(HWND, UINT, UINT, DWORD);
void poolSync();

void Update();

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINDOWSPROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT1));

	MSG msg;

	// Main message loop:
	while (true) //(GetMessage(&msg, nullptr, 0, 0))
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		/*if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/
		else
		{
			//Update();
		}
	}

	return (int)msg.wParam;
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDI_WINDOWSPROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		CreateBitmap();
		SetTimer(hWnd, 111, 60, KeyStateProc);
		GetClientRect(hWnd, &rectView);
		polyVertexCont.push_back({ bitBack.bmWidth * 2 / 5, bitBack.bmHeight * 2 / 5 });
		polyVertexCont.push_back({ bitBack.bmWidth * 3 / 5, bitBack.bmHeight * 2 / 5 });
		polyVertexCont.push_back({ bitBack.bmWidth * 3 / 5, bitBack.bmHeight * 3 / 5 });
		polyVertexCont.push_back({ bitBack.bmWidth * 2 / 5, bitBack.bmHeight * 3 / 5 });
		moveT = polyVertexCont[0];
		tmp = moveT;

		poolSync();

		//AllocConsole();
		//freopen("CONOUT$", "wt", stdout);
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		DrawBitmapDoubleBuffering(hWnd, hdc);
		DrawRectText(hdc);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DeleteBitmap();
		PostQuitMessage(0);
		//FreeConsole();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void DrawRectText(HDC hdc)
{
	TCHAR strTest[] = _T("달성률: ");
	TextOut(hdc, 30, 0, strTest, _tcslen(strTest));

	LONG polyArea = player.shoelace(polyVertexCont);
	double polyAreaPer = polyArea * 100 / (bitBack.bmWidth * bitBack.bmHeight) * 10 / 9;
	char buf[64];
	_itoa(polyAreaPer, buf, 10);
	TCHAR tBuf[64];
	mbstowcs(tBuf, buf, strlen(buf) + 1);
	tBuf[strlen(buf)] = '%';
	tBuf[strlen(buf) + 1] = '\0';
	TextOut(hdc, 100, 0, tBuf, _tcslen(tBuf));
}

void CreateBitmap()
{
	{
		hBackImage = (HBITMAP)LoadImage(NULL, TEXT("galsImage/수지1.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBackImage, sizeof(BITMAP), &bitBack);
	}
	{
		hBackImage2 = (HBITMAP)LoadImage(NULL, TEXT("galsImage/수지2.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		GetObject(hBackImage2, sizeof(BITMAP), &bitBack2);
	}
}

void DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	int bx, by;

	HDC hMemDC2;
	HBITMAP hOldBitmap2;

	hMemDC = CreateCompatibleDC(hdc);
	if (hDoubleBufferImage == NULL || sizeChangeFlag)
	{
		hDoubleBufferImage = CreateCompatibleBitmap(
			hdc, rectView.right, rectView.bottom);
		sizeChangeFlag = 0;
	}
	hOldBitmap = (HBITMAP)SelectObject(hMemDC,
		hDoubleBufferImage);

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

		HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 255));
		HBRUSH oldBrush = (HBRUSH)SelectObject(hMemDC2, myBrush);

		Polygon(hMemDC2, vertexPool, polyVertexCont.size());

		TransparentBlt(hMemDC, 0, 0, bx, by, hMemDC2,
			0, 0, bx, by, RGB(255, 0, 255));
		SelectObject(hMemDC2, oldBrush);
		DeleteObject(myBrush);

		player.show(hMemDC, moveT);

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}

	BitBlt(hdc, 0, 0, rectView.right, rectView.bottom,
		hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);

	DeleteDC(hMemDC);
}

void DeleteBitmap()
{
	DeleteObject(hBackImage);
	DeleteObject(hTransparentImage);
}

VOID CALLBACK KeyStateProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	bool downKeyFlag = false;
	if (GetKeyState('A') & 0x8000)
		downKeyFlag = true;

	player.collision(moveT, polyVertexCont, downKeyFlag);
	if (player.getCollidState())
	{
		player.mergePoly(polyVertexCont, polyLineCont);
		poolSync();
	}

	int movState = 4;
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		movState = 0;
		tmpVec = { -1, 0 };
	}
	else if (GetKeyState(VK_UP) & 0x8000)
	{
		movState = 1;
		tmpVec = { 0, -1 };
	}
	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		movState = 2;
		tmpVec = { 1, 0 };
	}
	else if (GetKeyState(VK_DOWN) & 0x8000)
	{
		movState = 3;
		tmpVec = { 0, 1 };
	}
	player.move(moveT, tmpVec, polyVertexCont, polyLineCont, movState, downKeyFlag, rectView);

	InvalidateRgn(hWnd, NULL, false);
}

void Update()
{
}

void poolSync()
{
	for (unsigned int i = 0; i < polyVertexCont.size(); i++)
	{
		vertexPool[i] = polyVertexCont[i];
		if (polyLineCont.size() == polyVertexCont.size()) break;

		polyLine tmpPL;
		tmpPL.startVertex = i;
		tmpPL.endVertex = (i + 1) % polyVertexCont.size();

		if (polyVertexCont[tmpPL.endVertex].x == polyVertexCont[tmpPL.startVertex].x)
			tmpPL.slope = 0;
		else
			tmpPL.slope =
			((polyVertexCont[tmpPL.endVertex].y - polyVertexCont[tmpPL.startVertex].y) /
			(polyVertexCont[tmpPL.endVertex].x - polyVertexCont[tmpPL.startVertex].x));
		if (tmpPL.slope == 0)
		{
			if (polyVertexCont[tmpPL.endVertex].x > polyVertexCont[tmpPL.startVertex].x)
				tmpPL.polyState = 3;
			else tmpPL.polyState = 1;
		}
		else if (tmpPL.slope == 0)
		{
			if (polyVertexCont[tmpPL.endVertex].y > polyVertexCont[tmpPL.startVertex].y)
				tmpPL.polyState = 0;
			else tmpPL.polyState = 2;
		}

		polyLineCont.push_back(tmpPL);
	}
}