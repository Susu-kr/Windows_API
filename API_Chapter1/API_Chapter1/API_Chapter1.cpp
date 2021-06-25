// API_Chapter1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _USE_MATH_DEFINES

#include "framework.h"
#include "API_Chapter1.h"
#include "Object.h"
#include "DLL.h"
#include <cmath>
#include <time.h>
#include <vector>

using namespace std;

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
	DrawRect(중심위치, 가로 크기, 세로 크기)
	DrawCircle(중심값, 반지름)
	DrawCircle(시작위치, 중심위치)
	DrawSunFlower(기준 원중심값, 기준원 반지름, 원 개수)
	DrawInputText(출력영역(Rect), 출력텍스트)
	DrawStar(중심위치, 반지름) 내부가 빈


	문1.
	클라이언트 영역에 마우스 왼쪽 버튼을 클릭하면 
	1. 해당 위치에 원을 생성
	2. 임의 방향으로 이동 (timer, random) 클라이언트 외곽에서 반사 (입사각, 반사각)
	3. 다른 오브젝트와 충돌시
		3.1 기본 서로 튕기도록 처리
		3.2 합체 - 상성관계에 따라 한 타입의 도형의 크기 증가, 다른 타입 도형 삭제
		3.3 분열 - 상성관계에 따라 작은 크기의 도형으로 분열 하도록
			별 > 원 > 사각형 > 별 .. 같은 상성끼리는 튕기도록.
		3.4 일정 크기 이상은 그냥 사라지고, 일정 크기 이하도 그냥 사라지도록 한다.
	4. 랜덤으로 사각형, 원, 별이 그러지도록
	5. 각 오브젝트를 회전 시킨다.

	+1. 각자 만든 리스트 적용
	-> 클래스 만들기

	CShape
	{
	private:
		좌표
		크기
		이동 스피드
		이동 방향
		
	public:	
		CShape();
		Update(); // 계속 업데이트
		Collision(); // 충돌
		Draw(); // 그려주기
	};

	CCircle : public CShape
	{
		// 기능 확장


	};

*/


typedef struct Point {
	double x;
	double y;
}Point;

void DrawGrid1(HDC hdc, double x1, double y1, double x2, double y2, double num);
void DrawGrid2(HDC hdc, double x, double y, double width, double height, double num);
void DrawRect(HDC hdc, int x, int y, int width, int height);
void DrawCircle1(HDC hdc, double x, double y, double radius);
void DrawCircle2(HDC hdc, double x1, double y1, double x2, double y2);
void DrawSunFlower(HDC hdc, double x, double y, double r, double num);
void DrawInputText(HDC hdc, RECT r, TCHAR * str);
void DrawStar(HDC hdc, int x, int y, int radius);
Point intersection(const Point* p1, const Point* p2, const Point* p3, const Point* p4);

void Change_Color(HDC hdc, int x, int y, int *flag, HBRUSH *hBrush);

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
	static int flag;
	static RECT rcRect;
	static List object;
	//static vector <CObject*> object;
	static POINT ptCircle;
	static POINT ptCircle2;

	const int rCircle = 100;
	const int moveSpeed = 50;
	static bool Flag;
	static BOOL bSelected;
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
		//CreateCaret(hWnd, NULL, 5, 15);
		//ShowCaret(hWnd);
		GetClientRect(hWnd, &rcRect);
		SetTimer(hWnd, 1, 10, NULL);

		//int breakPoint = 999;
		//count = 0;
		//yPos = 0;
		//flag = 0;
		//a++;
		//ptCircle.x += rCircle;
		//ptCircle.y += rCircle;
		//ptCircle2.x += rCircle;
		//ptCircle2.y += rCircle;
	}
	break;
	case WM_SIZE: // wParam : 윈도우창의 변경상황 여부, lParam(HIWORD : 윈도우 높이, LOWORD : 윈도우 너비)
	{
		//int breakpoint = 999;
		GetClientRect(hWnd, &rcRect);
		InvalidateRect(hWnd, NULL, true);
	}
	break;
	case WM_KEYDOWN:
		switch (wParam) {
		case 0x3A:
			flag = 0;
			break;
		case 0x3B:
			flag = 1;
			break;
		case 0x3C:
			flag = 2;
			break;

		}
			//case VK_RIGHT:
			//{
			//	Flag = true;
			//	ptCircle.x += moveSpeed;
			//	if (rcRect.right < ptCircle.x + rCircle) ptCircle.x -= moveSpeed;
			//}
			//	break;
			//case VK_LEFT:
			//{
			//	ptCircle.x -= moveSpeed;
			//	if (rcRect.left  > ptCircle.x - rCircle)	ptCircle.x += moveSpeed;
			//}
			//	break;
			//case VK_UP:
			//{
			//	ptCircle.y -= moveSpeed;
			//	if (rcRect.top  > ptCircle.y - rCircle)	ptCircle.y += moveSpeed;
			//}
			//	break;
			//case VK_DOWN:
			//{
			//	ptCircle.y += moveSpeed;
			//	if (rcRect.bottom < ptCircle.y + rCircle) ptCircle.y -= moveSpeed;
			//}
			//	break;
			//}
			//if (wParam == VK_UP) flag = 2;
			//else if (wParam == VK_DOWN) flag = 8;
			//else if (wParam == VK_LEFT) flag = 4;
			//else if (wParam == VK_RIGHT) flag = 6;
			//SetTimer(hWnd, 1, 100, NULL);
			//SetTimer(hWnd, 2, 100, NULL);
			//InvalidateRect(hWnd, NULL, true);
		break;
	//case WM_KEYUP:
	//	flag = 0;
	//	Flag = false;
	//	InvalidateRect(hWnd, NULL, true);
	//	break;
	//case WM_CHAR: {
	//	bKeyDown = true;
	//	if (wParam == VK_BACK && count > 0) count--;
	//	else if (wParam == VK_BACK && count == 0);
	//	else str[count++] = wParam;
	//	str[count] = NULL;
	//	InvalidateRect(hWnd, NULL, true); // true : 지우고 다시 그림
	//}
	//	break;
	case WM_TIMER:
		//switch (wParam)
		//{
		//case 1: // circle1
		//{
		//	ptCircle.x += moveSpeed;
		//	if (rcRect.right < ptCircle.x + rCircle) ptCircle.x -= moveSpeed;
		//}
		//break;
		//case 2: // circle2
		//{
		//	ptCircle2.x += moveSpeed;
		//	if (rcRect.right < ptCircle2.x + rCircle) ptCircle2.x -= moveSpeed;
		//	ptCircle2.y += moveSpeed;
		//	if (rcRect.bottom < ptCircle2.y + rCircle) ptCircle2.y -= moveSpeed;
		//}
		//	break;
		//default:
		//	break;
		//}
		switch (wParam) {
		case 1:
			for (int i = 0; i < object.ListSize(); i++) {
				object.OBINDEX(i)->Update();
			}
			for (int i = 0; i < object.ListSize(); i++) {
				object.OBINDEX(i)->Collision(rcRect);
			}
			for (int i = 0; i < object.ListSize(); i++) {
				for (int j = i+1; j < object.ListSize(); j++) {
					if(flag == 0) object.OBINDEX(i)->Collision(object.OBINDEX(j), NORMAL);
					else if (flag == 1) {
						object.OBINDEX(i)->Collision(object.OBINDEX(j), BIGGER);
						if (!object.OBINDEX(i)->GetActive()) object.removeIndex(i);
						else if (!object.OBINDEX(j)->GetActive()) object.removeIndex(j);
					}
					else if(flag == 2) object.OBINDEX(i)->Collision(object.OBINDEX(j), SMALLER);
				}
			}
			
			InvalidateRect(hWnd, NULL, true);
			break;
		}
	break;
	case WM_LBUTTONDOWN:
	{
		int num = rand() % 3;
		switch (num)
		{
		case 0:
			object.insertBack(new CCircle(LOWORD(lParam), HIWORD(lParam)));
			break;
		case 1:
			object.insertBack(new CRect(LOWORD(lParam), HIWORD(lParam)));
			break;
		case 2:
			object.insertBack(new CStar(LOWORD(lParam), HIWORD(lParam)));
			break;
		}

		//ptCircle.y = HIWORD(lParam);
		//ptCircle.x = LOWORD(lParam);
		//int breakpoint = 999;
		//bSelected = TRUE;
	}
	break;
	case WM_LBUTTONUP :
		bSelected = FALSE;
		break;
	case WM_RBUTTONDOWN:
	{
		//ptCircle2.y = HIWORD(lParam);
		//ptCircle2.x = LOWORD(lParam);
		//int breakpoint = 999;
		InvalidateRgn(hWnd, NULL, true);
	}
		break;
	case WM_MOUSEMOVE:
		//if (bSelected) {
		//	ptCircle.x = LOWORD(lParam);
		//	ptCircle.y = HIWORD(lParam);
		//	InvalidateRect(hWnd, NULL, true);
		//}
		break;
	case WM_POINTERUP:
		break;

	case WM_PAINT:
	{
		//a++;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		//RECT rect;
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		//TextOut(hdc, 100, 100, _T("Hello World"), _tcslen(_T("Hello World")));
		//rect.left = 200;
		//rect.top = 100;
		//rect.right = 400;
		//rect.bottom = 200;
		//SetTextColor(hdc, RGB(255, 0, 0));
		//DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
		//	DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		//DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
		//	DT_SINGLELINE | DT_LEFT );
		//DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
		//	DT_SINGLELINE | DT_RIGHT);
		//DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
		//	DT_SINGLELINE | DT_BOTTOM);
		//DrawText(hdc, _T("Hello World"), _tcslen(_T("Hello World")), &rect,
		//	DT_SINGLELINE | DT_RIGHT | DT_BOTTOM);
		
		//"Hello World \n (%s)님";
		//HPEN pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		//HPEN pen2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
		//rect = { 300, 100, 500, 300 };
		//DrawInputText(hdc, rect, str);
		//SelectObject(hdc, oldPen);
		//DeleteObject(pen);
		//HBRUSH brush, oldBrush;
		//brush = CreateSolidBrush(RGB(0, 255, 0));
		//brush = (HBRUSH)GetStockObject(NULL_BRUSH); // 뒷면도 보임 투명
		//oldBrush = (HBRUSH)SelectObject(hdc, brush);
		//Change_Color(hdc, 200, 200, &flag, &brush);
		//DrawCircle1(hdc, 200, 200, 100);
		//DrawCircle2(hdc, 400, 400, 450, 450);
		//DrawGrid1(hdc, 100, 100, 500, 500, 5);
		//DrawGrid2(hdc, 1000, 400, 300, 300, 5);
		//DrawCircle1(hdc, 1000, 400, 150);
		//DrawCircle2(hdc, 1150, 100, 1225, 175);
		//DrawSunFlower(hdc, 500,500,100, 100);
		//POINT point[10] = { 10, 150, 250, 30, 500, 150, 350, 300, 150, 300 };
		//Polygon(hdc, point, 5);
		//DrawStar(hdc, 500, 500, 200);
		//DrawStar(hdc, 500, 500, 150);
		//DrawStar(hdc, 500, 500, 100);
		//HPEN oldPen = (HPEN)SelectObject(hdc, pen);
		//if (Flag) SelectObject(hdc, GetStockObject(LTGRAY_BRUSH));
		//Ellipse(hdc, ptCircle.x - 100, ptCircle.y - 100,
		//	ptCircle.x + 100 , ptCircle.y + 100);
		//SelectObject(hdc, oldPen);
		//HPEN oldPen2 = (HPEN)SelectObject(hdc, pen2);
		//Ellipse(hdc, ptCircle2.x - 100, ptCircle2.y - 100,
		//	ptCircle2.x + 100, ptCircle2.y + 100);
		//SelectObject(hdc, oldPen2);
		//DeleteObject(pen);
		//DeleteObject(pen2);
		
		for (int i = 0; i < object.ListSize(); i++) {
			object.OBINDEX(i)->Draw(hdc);
		}

		//for (CObject* temp : object)
		//	temp->Draw(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		//HideCaret(hWnd);
		//DestroyCaret();
		PostQuitMessage(0);
		for (int i = 0; i < object.ListSize(); i++) {
			object.removeFront();
		}
		//for (CObject * temp : object)
		//	delete temp;
		KillTimer(hWnd, 1);
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

void DrawRect(HDC hdc, int x, int y, int width, int height) {
	Rectangle(hdc, x - width/2, y - height/2, x + width/2, y + height/2);
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

void DrawInputText(HDC hdc, RECT r, TCHAR *str) {
	Rectangle(hdc, r.left, r.top, r.right, r.bottom);
	r = { r.left + 10, r.top + 10, r.right - 10, r.bottom - 10 };
	DrawText(hdc, str, _tcslen(str), &r, DT_TOP | DT_LEFT | DT_SINGLELINE);
	SIZE size;
	GetTextExtentPoint(hdc, str, _tcslen(str), &size);
	SetCaretPos(size.cx + r.left, r.top);
}

void DrawStar(HDC hdc, int x, int y, int radius) {
	double degree = 2 * M_PI / 5;
	Point point[5];
	Point dup_p[5];
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


Point intersection(const Point* p1, const Point* p2, const Point* p3, const Point* p4) {
	Point ret;
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

void Change_Color(HDC hdc, int x, int y, int *flag, HBRUSH *hBrush) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int val = (3 * i) + j + 1; // for문 횟수
			if (val % 2 == 0) {
				TCHAR tmp[20];
				switch (val)
				{
				case 2:
					_stprintf_s(tmp, _T("위쪽"));
					break;
				case 4:
					_stprintf_s(tmp, _T("왼쪽"));
					break;
				case 6:
					_stprintf_s(tmp, _T("오른쪽"));
					break;
				case 8:
					_stprintf_s(tmp, _T("아래쪽"));
					break;
				}

				if (*flag == val) {
					*hBrush = CreateSolidBrush(RGB(255, 0, 0));
					SelectObject(hdc, *hBrush);
				}
				DrawRect(hdc, x + j * 100, y + i * 200, 100, 200);
				TextOut(hdc, x + j * 100-25, y + i * 200, tmp, _tcslen(tmp));
				SelectObject(hdc, GetStockObject(WHITE_BRUSH));
			}
		}
	}
}