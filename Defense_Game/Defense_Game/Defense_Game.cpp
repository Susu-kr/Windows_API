// Defense_Game.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "framework.h"
#include "Defense_Game.h"
#include "Game.h"
#include "Object.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>

#define MAX_LOADSTRING 100

using namespace std;

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
    LoadStringW(hInstance, IDC_DEFENSEGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFENSEGAME));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFENSEGAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DEFENSEGAME);
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
	srand(unsigned(time(NULL)));

	static GAME		G;
	static RECT		R;
	static TCHAR	Player[20];
	static TCHAR	read[2];
	static int		count;
	static SIZE		temp;
	static float	scale;
	DWORD	size;

	static PLAY *Cannon;
	static vector <Bullet*> bullet;
	static vector <Wall*> wall;
	static vector <Enemy*> enemy;

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
			GetClientRect(hWnd, &R);
			SetTimer(hWnd, 1, 10, NULL);
			count = 0;
		}
		break;
	case WM_SIZE:
		{
			GetClientRect(hWnd, &R);
			InvalidateRect(hWnd, NULL, true);
		}
		break;
	case WM_CHAR:
		{
		if (G.GetMode() == LOBBY) {
				if (wParam == VK_BACK && count > 0) count--;
				else if (wParam == VK_BACK && count == 0);
				else if (wParam == VK_RETURN) {
					HideCaret(hWnd);
					DestroyCaret();
					G.SetMode(INGAME);
					scale = 100;
					int x = R.left - (scale / 2), y = R.bottom - (scale / 4);
					while (x < R.right) {
						x += scale;
						wall.push_back(new Wall(x, y));
						x += 1;
					}
				}
				else Player[count++] = wParam;
				Player[count] = NULL;
				if (count == 19) count--;
				InvalidateRect(hWnd, NULL, true);
			}
		}
		break;

	case WM_TIMER:
		{
			switch (wParam)
			{
			case 1:
				Cannon->Update();
				for (int i = 0; i < wall.size(); i++) {
					wall[i]->Update();
				}
				for (int i = 0; i < bullet.size(); i++) {
					bullet[i]->Update();
				}
				for (int i = 0; i < bullet.size(); i++) {
					bullet[i]->Collision(R);
					if (bullet[i]->GetActive() == false) bullet.erase(bullet.begin() + i);
				}
				break;
			}
			InvalidateRect(hWnd, NULL, true);
		}
		break;
	case WM_KEYDOWN:
		{
			if (G.GetMode() == INGAME) {
				switch (wParam)
				{
				case VK_LEFT:
					if (Cannon->GetDegree() < 170 * M_PI / 180)Cannon->SetDegree(Cannon->GetDegree() + M_PI / 90);
					break;
				case VK_RIGHT:
					if (Cannon->GetDegree() > M_PI / 18)Cannon->SetDegree(Cannon->GetDegree() - M_PI / 90);
					break;
				case VK_SPACE:
					bullet.push_back(new Bullet(Cannon->GetShotPos().x, Cannon->GetShotPos().y, Cannon->GetDegree()));
					break;
				}
			}
		}
		break;
    case WM_PAINT:
        {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
			switch (G.GetMode()) {
			case LOBBY:
				G.Lobby(hdc, R, Player, temp);
				break;
			case INGAME:
				for (int i = 0; i < wall.size(); i++)
					wall[i]->Draw(hdc);
				for (int i = 0; i < bullet.size(); i++)
					bullet[i]->Draw(hdc);
				//for (int i = 0; i < enemy.size(); i++)
				//	enemy[i]->Draw(hdc);
				break;
			case ENDGAME:
				G.EndGame(hdc, R, Player);
				return 0;
			}
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		delete Cannon;
		for (STRUCT * temp : wall)
			delete temp;
		for (STRUCT * temp : bullet)
			delete temp;
		for (STRUCT * temp : enemy)
			delete temp;
		KillTimer(hWnd, 1);
		//KillTimer(hWnd, 2);
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
