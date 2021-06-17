// API_Chapter1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _USE_MATH_DEFINES

#include "framework.h"
#include "API_Chapter1.h"
#include <cmath>

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

/*
	격자 그리기 함수 DrawGrid() 구현
	인자가 시작 위치, 끝위치 LeftTop, RightBottom ... RECT, POINT
	DrawGrid(시작위치, 끝위치, 선 개수)
	DrawGrid(중심위치, 간격, 선의 개수)
*/
void DrawGrid1(HDC hdc, double x1, double y1, double x2, double y2, double num);
void DrawGrid2(HDC hdc, double x, double y, double width, double height, double num);
/*
	원 그리기 함수 DrawCircle() 구현
	DrawCircle(중심값, 반지름)
	DrawCircle(시작위치, 중심위치)
*/
void DrawCircle1(HDC hdc, double x, double y, double radius);
void DrawCircle2(HDC hdc, double x1, double y1, double x2, double y2);
/*
	해바라기
	DrawSunFlower(기준 원중심값, 기준원 반지름, 원 개수)
*/
void DrawSunFlower(HDC hdc, double x, double y, double r, double num);


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
	LoadStringW(hInstance, IDC_APICHAPTER1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APICHAPTER1));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APICHAPTER1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_APICHAPTER1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
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
	int a = 0;
	static bool bKeyDown = false;
	static TCHAR str[1000];
	static int count, yPos;
	static SIZE size;
	switch (message)
	{
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
	case WM_CREATE:
	{
		CreateCaret(hWnd, NULL, 5, 15);
		ShowCaret(hWnd);
		int breakPoint = 999;
		count = 0;
		yPos = 0;
		a++;
	}
	break;

	case WM_KEYDOWN:
		break;
	case WM_KEYUP:
		break;
	case WM_CHAR: {
		bKeyDown = true;
		if (wParam == VK_BACK && count > 0) count--;
		else if (wParam == VK_BACK && count == 0);
		else str[count++] = wParam;
		str[count] = NULL;
		InvalidateRect(hWnd, NULL, true); // true : 지우고 다시 그림
	}
		break;

	case WM_PAINT:
	{
		a++;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		TextOut(hdc, 100, 100, _T("Hello World"), _tcslen(_T("Hello World")));
		
		RECT rect;
		rect.left = 200;
		rect.top = 100;
		rect.right = 400;
		rect.bottom = 200;
		SetTextColor(hdc, RGB(255, 0, 0));
		DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
			DT_SINGLELINE | DT_LEFT );
		DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
			DT_SINGLELINE | DT_RIGHT);
		DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
			DT_SINGLELINE | DT_BOTTOM);
		DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
			DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);

		//"Hello World \n (%s)님";

		rect = { 0, 0, 1000, 1000 };
		if (bKeyDown) {
			GetTextExtentPoint(hdc, str, _tcslen(str), &size);
			TextOut(hdc, 0, 0, str, _tcslen(str));
			//DrawText(hdc, str, _tcslen(str), &rect, DT_TOP | DT_LEFT);
			SetCaretPos(size.cx, 0);
		}

		//DrawCircle1(hdc, 200, 200, 100);
		//DrawCircle2(hdc, 400, 400, 450, 450);
		DrawGrid1(hdc, 100, 100, 500, 500, 5);

		DrawGrid2(hdc, 1000, 400, 300, 300, 5);
		DrawCircle1(hdc, 1000, 400, 150);
		DrawCircle2(hdc, 1150, 100, 1225, 175);

		DrawSunFlower(hdc, 500,500,100, 15);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
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

void DrawGrid1(HDC hdc, double x1, double y1, double x2, double y2, double num) {
	// 수평
	double height = y1;
	for (double i = 0; i < num; i++) {
		MoveToEx(hdc, (int)x1, (int)height, NULL);
		LineTo(hdc, (int)x2, (int)height);
		height += (int)(y2-y1) / (int)(num-1);
	}
	// 수직
	double width = x1;
	for (double i = 0; i < num; i++) {
		MoveToEx(hdc, (int)width, (int)y1, NULL);
		LineTo(hdc, (int)width, (int)y2);
		width += (int)(x2-x1) / (int)(num-1);
	}
}
void DrawGrid2(HDC hdc, double x, double y, double width, double height, double num) {
	// 수평
	double x1 = x - width;
	double x2 = x + width;
	double hori = y - height;
	for (double i = 0; i < num; i++) {
		MoveToEx(hdc, (int)x1, (int)hori, NULL);
		LineTo(hdc, (int)x2, (int)hori);
		hori += (int)(2 * height) / (int)(num-1);
	}
	hori -= (2 * height) / (num - 1);
	// 수직
	double boundary = x1;
	for (double i = 0; i < num; i++) {
		MoveToEx(hdc, (int)boundary, (int)(y - height), NULL);
		LineTo(hdc, (int)boundary, (int)hori);
		boundary += (int)(x2 - x1) / (int)(num-1);
	}
}

void DrawCircle1(HDC hdc, double x, double y, double radius) {
	Ellipse(hdc, (int)(x - radius), (int)(y - radius), (int)(x + radius), (int)(y + radius));
}
void DrawCircle2(HDC hdc, double x1, double y1, double x2, double y2) {
	Ellipse(hdc, (int)x1, (int)y1, (int)x2 + (int)(x2 - x1), (int)y2 + (int)(y2 - y1));
}

void DrawSunFlower(HDC hdc, double x, double y, double r, double num) {
	if (num > 3) {
		double degree = 2 * M_PI / num;
		double s_r = abs((r * sin(degree / 2) / (1 - sin(degree / 2))));
		double temp_d = 0;
		for (double i = 0; i < num; i++) {
			DrawCircle1(hdc, x + (cos(temp_d) * (r + s_r))
				, y + (sin(temp_d) * (r + s_r)), s_r);
			temp_d += degree;
		}
		DrawCircle1(hdc, x, y, r);
	}
}