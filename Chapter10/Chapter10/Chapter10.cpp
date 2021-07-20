// Chapter10.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _USE_MATH_DEFINES

#include "framework.h"
#include "Chapter10.h"

// << :
#include <cmath>
#include <process.h>
#include <time.h>
#define MAX_THREAD_NUM 50
int XPOS, YPOS;
int r, g, b;
void ThreadProc();
HWND hWnd;
void DrawStar(HDC hdc, int x, int y, int radius);
POINT intersection(const POINT* p1, const POINT* p2, const POINT* p3, const POINT* p4);
// >>
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
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
    LoadStringW(hInstance, IDC_CHAPTER10, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHAPTER10));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHAPTER10));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHAPTER10);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HANDLE hThread[MAX_THREAD_NUM];
	static RECT rect;
	static int count = 0;

    switch (message)
    {
	case WM_CREATE:
		GetClientRect(hWnd, &rect);
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
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
	case WM_LBUTTONDOWN:
	{
		XPOS = rand() % rect.right;
		YPOS = rand() % rect.bottom;
		r = rand() % 256;
		g = rand() % 256;
		b = rand() % 256;
		hThread[count] = (HANDLE)_beginthreadex(NULL, 0,
			(unsigned int(__stdcall *)(void *))ThreadProc,
			NULL, 0, NULL);
		count++;

	}
		break;
    //case WM_PAINT:
    //    {
    //        hdc = BeginPaint(hWnd, &ps);
    //        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
    //        EndPaint(hWnd, &ps);
    //    }
    //    break;
    case WM_DESTROY:
		for (int i = 0; i < MAX_THREAD_NUM; i++)
			CloseHandle(hThread[i]);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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


void ThreadProc()
{
	HDC hdc;
	srand(time(NULL));
	int xP, yP;
	hdc = GetDC(hWnd);
	xP = XPOS;
	yP = YPOS;
	SelectObject(hdc, CreateSolidBrush(RGB(r, g, b)));
	int flag = rand() % 3;
	switch (flag) {
	case 0:
		Ellipse(hdc, xP - 10, yP - 10, xP + 10, yP + 10);
		break;
	case 1:
		Rectangle(hdc, xP - 10, yP - 10, xP + 10, yP + 10);
		break;
	case 2:
		DrawStar(hdc, xP, yP, 10);
		break;
	}
	for (int i = 0; i < 100; i++) {
		Sleep(1000);
		switch (flag) {
		case 0:
			Ellipse(hdc, xP - 10, yP - 10, xP + 10, yP + 10);
			break;
		case 1:
			Rectangle(hdc, xP - 10, yP - 10, xP + 10, yP + 10);
			break;
		case 2:
			DrawStar(hdc, xP, yP, 10);
			break;
		}
		SelectObject(hdc, CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256)));
	}
	ReleaseDC(hWnd, hdc);
	return;
}

void DrawStar(HDC hdc, int x, int y, int radius) {
	double degree = 2 * M_PI / 5;
	POINT point[5];
	POINT dup_p[5];
	double temp_d = 11;
	int cnt = 0;
	for (int i = 0; i < 5; i++) {
		point[i].x = x + (cos(temp_d) * (radius));
		point[i].y = y + (sin(temp_d) * (radius));
		temp_d += degree;
	}

	dup_p[0] = intersection(&point[0], &point[2], &point[1], &point[4]); 	// 1.5 = 1 - 3 2 - 5
	dup_p[1] = intersection(&point[0], &point[2], &point[1], &point[3]);	// 2.5 = 1 - 3 2 - 4
	dup_p[2] = intersection(&point[1], &point[3], &point[2], &point[4]);    // 3.5 = 2 - 4 3 - 5
	dup_p[3] = intersection(&point[0], &point[3], &point[2], &point[4]);	// 4.5 = 1 - 4 3 - 5
	dup_p[4] = intersection(&point[0], &point[3], &point[1], &point[4]);    // 5.5 = 1 - 4 2 - 5

	POINT star[10];

	for (int i = 0; i < 5; i++) {
		star[i * 2].x = point[i].x;
		star[i * 2].y = point[i].y;
		star[i * 2 + 1].x = dup_p[i].x;
		star[i * 2 + 1].y = dup_p[i].y;
	}
	Polygon(hdc, star, 10);
}

POINT intersection(const POINT* p1, const POINT* p2, const POINT* p3, const POINT* p4) {
	POINT ret;
	/*
				(x1y2 - y1x2)(x3 - x4) - (x1 - x2)(x3y4 - y3x4)			(x1y2 - y1x2)(y3 - y4) - (y1 - y2)(x3y4 - y3x4)
	(px,py) =	------------------------------------------------		------------------------------------------------
					(x1 - x2)(y3 - y4) - (y1 - y2)(x3 - x4)			,		(x1 - x2)(y3 - y4) - (y1 - y2)(x3 - x4)
	*/
	ret.x = ((p1->x*p2->y - p1->y*p2->x)*(p3->x - p4->x) - (p1->x - p2->x)*(p3->x*p4->y - p3->y*p4->x))
		/ ((p1->x - p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x - p4->x));

	ret.y = ((p1->x*p2->y - p1->y*p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x*p4->y - p3->y*p4->x))
		/ ((p1->x - p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x - p4->x));

	return ret;
}