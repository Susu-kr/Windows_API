// Chapter4.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Chapter4.h"
#include <fstream>
#include <cmath>
#include <time.h>
#include <commdlg.h>
#include <CommCtrl.h>

// >> : GDI+
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
// << 
#pragma comment(lib, "msimg32.lib")

#define MAX_LOADSTRING 100

void OutFromFile(TCHAR filename[], HWND hWnd);


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// image
HBITMAP				hBackImage;
BITMAP				bitBack;
// transparent image
HBITMAP				hTransparentImage;
BITMAP				bitTransparent;
// Animation
HBITMAP				hAniImage;
BITMAP				bitAni;
const int			SPRITE_SIZE_X = 57;
const int			SPRITE_SIZE_Y = 52;
//const int			SPRITE_SIZE_X = 32;
//const int			SPRITE_SIZE_Y = 32;
POINT				AniPos;
int					flag;
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

// >> : GDI+
ULONG_PTR			g_GdiToken;

void				Gdi_Init();
void				Gdi_Draw(HDC hdc);
void				Gdi_End();
void				OnGdi_Paint(HDC hdc);

// <<



void				CreateBitmap();
void				DrawBitmap(HWND hWnd, HDC hdc);
void				DeleteBitmap();
void				UpdateFrame(HWND hWnd);
VOID CALLBACK		AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime);
// <<

BOOL CALLBACK		MyDlg_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK		CONTROL_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK		CONTROL_Proc2(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
static HWND			hModallessDlg;

void MakeColumn(HWND hDlg);
void InsertData(HWND hDlg);
int SelectItem(HWND hDlg, LPARAM lParam);
void ModifyItem(HWND hDlg, int selection);
void DeleteItem(HWND hDlg, int selection);

BOOL CALLBACK		ListControl_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

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
    LoadStringW(hInstance, IDC_CHAPTER4, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CHAPTER4));

    MSG msg;

	Gdi_Init();

    // 기본 메시지 루프입니다:
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}

	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			Update();
		}
	}

	Gdi_End();

    return (int) msg.wParam;
}

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
		AniPos.x += 10;
		flag = 6;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		AniPos.x -= 10;
		flag = 4;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		AniPos.y -= 10;
		flag = 8;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		AniPos.y += 10;
		flag = 2;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && GetAsyncKeyState(VK_UP) & 0x8000) {
		flag = 9;
		return;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && GetAsyncKeyState(VK_DOWN) & 0x8000) {
		flag = 3;
		return;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000 && GetAsyncKeyState(VK_UP) & 0x8000) {
		flag = 7;
		return;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000 && GetAsyncKeyState(VK_DOWN) & 0x8000) {
		flag = 1;
		return;
	}
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CHAPTER4));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CHAPTER4);
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
	static RECT			rcRect;
	OPENFILENAME		OFN;
	TCHAR				str[100], lpstrFile[100] = _T("");
	TCHAR				filter[] = _T("Every File(*.*)\0*.*\0Text File\0*.txt;*.doc\0");

	// color box
	CHOOSECOLOR			COLOR;
	static COLORREF		tmp[16], color;
	HBRUSH				hBrush, OldBrush;
	int					i;

    switch (message)
    {
	case WM_CREATE:
		{
			flag = 0;
			GetClientRect(hWnd, &rectView);
			CreateBitmap();
			SetTimer(hWnd, 123, 50, AniProc);
		}
		break;
	case WM_SIZE:
		{
			GetClientRect(hWnd, &rcRect);
			InvalidateRect(hWnd, NULL, false);
		}
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_TIMER:
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
			case ID_FUNC1:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_MYDIALOG), hWnd, MyDlg_Proc);
			}
				break;
			case ID_FUNC2:
			{
				int ans = MessageBox(hWnd, _T("합체설정으로 작동합니다."), _T("기능 선택"), MB_YESNOCANCEL);
				switch (ans) {
				case IDYES:
					MessageBox(hWnd, _T("YES 버튼 선택"), _T("버튼 확인"), MB_OK);
					break;
				case IDNO:
					MessageBox(hWnd, _T("NO 버튼 선택"), _T("버튼 확인"), MB_OK);
					break;
				case IDCANCEL:
					MessageBox(hWnd, _T("CANCLE 버튼 선택"), _T("버튼 확인"), MB_OK);
					break;
				}
			}
				break;
			case ID_FUNC3:
				MessageBox(hWnd, _T("분열설정으로 작동합니다."), _T("기능 선택"), MB_OK);
				break;
			
			case ID_FUNC4:
				MessageBox(hWnd, _T("기능4설정으로 작동합니다."), _T("기능 선택"), MB_OK);
				break;
			case ID_FUNC5:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_CONTROL_DIALOG), hWnd, CONTROL_Proc);
				break;
			case ID_FUNC6:
			{
				if (!IsWindow(hModallessDlg))
				{
					hModallessDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CONTROL_DIALOG), hWnd, CONTROL_Proc2);
					ShowWindow(hModallessDlg, SW_SHOW);
				}
			}
				break;
			case ID_FUNC7:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_LIST_CONTROL), hWnd, ListControl_Proc);
			}
			break;
			case ID_FILEOPEN:
			{
				memset(&OFN, 0, sizeof(OPENFILENAME));
				OFN.lStructSize = sizeof(OPENFILENAME);
				OFN.hwndOwner = hWnd;
				OFN.lpstrFile = filter;
				OFN.lpstrFile = lpstrFile;
				OFN.nMaxFile = 100;
				OFN.lpstrInitialDir = _T(".");
				if (GetOpenFileName(&OFN) != 0) {
					OutFromFile(OFN.lpstrFile, hWnd);
				}
				break;
			}
			case ID_COLORDLG:
			{
				for (i = 0; i < 16; i++) tmp[i] = RGB(rand() % 256, rand() % 256, rand() % 256);
				memset(&COLOR, 0, sizeof(CHOOSECOLOR));
				COLOR.lStructSize = sizeof(CHOOSECOLOR);
				COLOR.hwndOwner = hWnd;
				COLOR.lpCustColors = tmp;
				COLOR.Flags = CC_FULLOPEN;
				if (ChooseColor(&COLOR) != 0) {
					color = COLOR.rgbResult;
					InvalidateRgn(hWnd, NULL, TRUE);
				}
				
			}
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_KEYDOWN:
	{
		switch (wParam) {
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

		}
	}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			//hBrush = CreateSolidBrush(color);
			//OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			//Ellipse(hdc, 10, 10, 200, 200);
			//SelectObject(hdc, OldBrush);
			//DeleteObject(hBrush);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

			//DrawBitmap(hWnd, hdc);
			//DrawRectText(hdc);
			DrawBitmapDoubleBuffering(hWnd, hdc);
			EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		DeleteBitmap();
		KillTimer(hWnd, 123);
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

void DrawRectText(HDC hdc)
{
	static int yPos = 0;
	TCHAR strTest[] = _T("이미지 출력");
	TextOut(hdc, 10, yPos, strTest, _tcslen(strTest));
	yPos += 5;
	if (yPos > rectView.bottom) yPos = 0;
}

void CreateBitmap()
{
	// >> : back image
	hBackImage = (HBITMAP)LoadImage(NULL, TEXT("images/수지.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBackImage, sizeof(BITMAP), &bitBack);
	// << 

	// >> : Transparent Image
	hTransparentImage = (HBITMAP)LoadImage(NULL, TEXT("images/sigong.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hTransparentImage, sizeof(BITMAP), &bitTransparent);
	// << :

	// >> : Animation
	hAniImage = (HBITMAP)LoadImage(NULL, TEXT("images/zero_run.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hAniImage, sizeof(BITMAP), &bitAni);
	RUN_FRAME_MAX = bitAni.bmWidth / SPRITE_SIZE_X - 1;
	RUN_FRAME_MIN = 2;
	curframe = RUN_FRAME_MIN;
	AniPos = { 200 ,400 };

	//hAniImage = (HBITMAP)LoadImage(NULL, TEXT("images/8D.bmp"),
	//	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	//GetObject(hAniImage, sizeof(BITMAP), &bitAni);

	//RUN_FRAME_MAX = bitAni.bmWidth / SPRITE_SIZE_X / 2 - 1;
	//RUN_FRAME_MIN = 0;
	//curframe = RUN_FRAME_MIN;
	//AniPos = { 200 ,400 };
	// << :
}

void DrawBitmap(HWND hWnd, HDC hdc)
{
	HDC			hMemDC; // Double Buffering
	HBITMAP		hOldBitmap;
	int bx, by;

	// back image
	{
		hMemDC = CreateCompatibleDC(hdc);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackImage);
		bx = bitBack.bmWidth;
		by = bitBack.bmHeight;

		BitBlt(hdc, 0, 0, bx, by, hMemDC, 0, 0, SRCCOPY);
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
	//

	// transparent image
	{
		hMemDC = CreateCompatibleDC(hdc);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hTransparentImage);
		bx = bitTransparent.bmWidth;
		by = bitTransparent.bmHeight;

		BitBlt(hdc, 100, 200, bx, by, hMemDC, 0, 0, SRCCOPY);
		TransparentBlt(hdc, 200, 200, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
	//

	// animation
	{
		hMemDC = CreateCompatibleDC(hdc);
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hAniImage);
		bx = SPRITE_SIZE_X;
		by = SPRITE_SIZE_Y;
		switch (flag)
		{
		case 1:
			Start_Pos_X = (curframe + 6) * SPRITE_SIZE_X;
			Start_Pos_Y = 1 * SPRITE_SIZE_Y;
			break;
		case 2:
			Start_Pos_X = curframe * SPRITE_SIZE_X;
			Start_Pos_Y = 0 * SPRITE_SIZE_Y;
			break;
		case 3:
			Start_Pos_X = (curframe + 6) * SPRITE_SIZE_X;
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
		case 7:
			Start_Pos_X = (curframe + 6) * SPRITE_SIZE_X;
			Start_Pos_Y = 2 * SPRITE_SIZE_Y;
			break;
		case 8:
			Start_Pos_X = curframe * SPRITE_SIZE_X;
			Start_Pos_Y = 3 * SPRITE_SIZE_Y;
			break;
		case 9:
			Start_Pos_X = (curframe + 6) * SPRITE_SIZE_X;
			Start_Pos_Y = 3 * SPRITE_SIZE_Y;
			break;
		default:
			break;
		}
		TransparentBlt(hdc, AniPos.x, AniPos.y, bx, by, hMemDC,
			Start_Pos_X, Start_Pos_Y, bx, by, RGB(255, 255, 255));
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
	// 
}

void DeleteBitmap()
{
	DeleteObject(hBackImage);
	DeleteObject(hTransparentImage);
	DeleteObject(hAniImage);
}

void UpdateFrame(HWND hWnd)
{
	curframe++;
	if (curframe > RUN_FRAME_MAX)
		curframe = RUN_FRAME_MIN;
}

VOID CALLBACK AniProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
	curframe++;
	if (curframe > RUN_FRAME_MAX)
		curframe = RUN_FRAME_MIN;
	InvalidateRect(hWnd, NULL, false);
}

BOOL CALLBACK MyDlg_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static int Check[3], Radio;
	TCHAR hobby[][30] = { _T("독서"), _T("음악감상") , _T("게임") };
	TCHAR sex[][30] = { _T("여성") , _T("남성") };
	TCHAR output[256];

	switch (iMsg) {
	case WM_INITDIALOG: {
		HWND hWndBtn = GetDlgItem(hDlg, IDC_PAUSE_BTN);
		EnableWindow(hWndBtn, false);
	}
	{
		CheckRadioButton(hDlg, IDC_RADIO_FEMALE, IDC_RADIO_MALE, IDC_RADIO_FEMALE);
	}
		return 1;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam)) {
		case IDC_CHECK_READ:
			Check[0] = 1 - Check[0];
			break;
		case IDC_CHECK_MUSIC:
			Check[1] = 1 - Check[1];
			break;
		case IDC_CHECK_GAME:
			Check[2] = 1 - Check[2];
			break;
		case IDC_RADIO_FEMALE:
			Radio = 0;
			break;
		case IDC_RADIO_MALE:
			Radio = 1;
			break;
		case IDC_OUTPUT_BTN:
			_stprintf_s(output, _T("선택한 취미는 %s %s %s 입니다.\r\n")
				 _T("선택한 성별은 %s 입니다.")
				, Check[0] ? hobby[0] : _T("")
				, Check[1] ? hobby[1] : _T("")
				, Check[2] ? hobby[2] : _T("")
				, sex[Radio]);
			SetDlgItemText(hDlg, IDC_EDIT_OUTPUT, output);
		case IDC_COPY_BTN:
		{
			TCHAR text[128];
			GetDlgItemText(hDlg, IDC_INPUT_EDIT, text, 128);
			SetDlgItemText(hDlg, IDC_OUTPUT_EDIT, text);
			SetDlgItemText(hDlg, IDC_INPUT_EDIT, _T(""));

		}
			break;
		case IDC_CLEAR_BTN:
			SetDlgItemText(hDlg, IDC_INPUT_EDIT, _T(""));
			SetDlgItemText(hDlg, IDC_OUTPUT_EDIT, _T(""));

			break;

		case IDC_START_BTN:
			SetDlgItemText(hDlg, IDC_MYDLG_STATIC, _T("시작"));
			{
				HWND hWndBtn = GetDlgItem(hDlg, IDC_START_BTN);
				EnableWindow(hWndBtn, false);

				hWndBtn = GetDlgItem(hDlg, IDC_PAUSE_BTN);
				EnableWindow(hWndBtn, true);
			}
			break;
		case IDC_PAUSE_BTN:
			SetDlgItemText(hDlg, IDC_MYDLG_STATIC, _T("정지"));
			{
				HWND hWndBtn = GetDlgItem(hDlg, IDC_START_BTN);
				EnableWindow(hWndBtn, true);

				hWndBtn = GetDlgItem(hDlg, IDC_PAUSE_BTN);
				EnableWindow(hWndBtn, false);
			}
			break;
		case IDC_STOP_BTN:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
		break;
	}
	return 0;
}

// 컨트롤 전용
BOOL CALLBACK CONTROL_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCombo;
	static int	selection;
	TCHAR		name[20];

	static HWND hList;
	static int	selection2;

	switch (iMsg) {
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO_LIST);
		hList = GetDlgItem(hDlg, IDC_LIST_NAME);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDOK:
				EndDialog(hDlg, 0);
				return 0;

			case IDC_INSERT_BTN: 
			{
				GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
				if (_tcscmp(name, _T(""))) 
				{
					SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)name);
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)name);
					SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
					return 0;
				}
			}
			break;
			case IDC_DELETE_BTN:
			{
				SendMessage(hCombo, CB_DELETESTRING, selection, 0);
				return 0;
			}
			break;
			case IDC_COMBO_LIST:
			{
				if (HIWORD(wParam) == CBN_SELCHANGE) {
					selection = (int)SendMessage(hCombo, CB_GETCURSEL, 0, 0);
				}
			}
			break;
			case IDC_DELETE_BTN2:
			{
				SendMessage(hList, LB_DELETESTRING, selection2, 0);
				return 0;
			}
			break;
			case IDC_LIST_NAME:
			{
				if (HIWORD(wParam) == LBN_SELCHANGE) {
					selection2 = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
				}
			}
			break;
		}
	}
	return 0;
}
// : 모달 다이얼로그 전용
BOOL CALLBACK CONTROL_Proc2(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hCombo;
	static int	selection;
	TCHAR		name[20];

	static HWND hList;
	static int	selection2;

	switch (iMsg) {
	case WM_INITDIALOG:
		hCombo = GetDlgItem(hDlg, IDC_COMBO_LIST);
		hList = GetDlgItem(hDlg, IDC_LIST_NAME);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, 0);
			DestroyWindow(hModallessDlg);
			hModallessDlg = NULL;
			return 0;

		case IDC_INSERT_BTN:
		{
			GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
			if (_tcscmp(name, _T("")))
			{
				SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)name);
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)name);
				SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
				return 0;
			}
		}
		break;
		case IDC_DELETE_BTN:
		{
			SendMessage(hCombo, CB_DELETESTRING, selection, 0);
			return 0;
		}
		break;
		case IDC_COMBO_LIST:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				selection = (int)SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			}
		}
		break;
		case IDC_DELETE_BTN2:
		{
			SendMessage(hList, LB_DELETESTRING, selection2, 0);
			return 0;
		}
		break;
		case IDC_LIST_NAME:
		{
			if (HIWORD(wParam) == LBN_SELCHANGE) {
				selection2 = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
			}
		}
		break;
		}
	}
	return 0;
}
void MakeColumn(HWND hDlg)
{
	LPTSTR		name[2] = { (LPTSTR)_T("이름"), (LPTSTR)_T("전화번호") };
	LVCOLUMN	lvCol = { 0, };
	HWND		hList;
	int			i;
	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
	lvCol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvCol.fmt = LVCFMT_LEFT;

	for (i = 0; i < 2; i++) {
		lvCol.cx = 90;
		lvCol.iSubItem = i;
		lvCol.pszText = name[i];
		SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)i, (LPARAM)&lvCol);
	}
}

void InsertData(HWND hDlg)
{
	int			count;
	LVITEM		item;
	HWND		hList;
	TCHAR		name[20], phone[20];

	GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
	SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
	if (_tcscmp(name, _T("")) == 0) return;

	GetDlgItemText(hDlg, IDC_EDIT_PHONE, phone, 20);
	SetDlgItemText(hDlg, IDC_EDIT_PHONE, _T(""));


	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
	count = ListView_GetItemCount(hList);
	item.mask = LVIF_TEXT;
	item.iItem = count;
	item.iSubItem = 0;
	item.pszText = name;
	ListView_InsertItem(hList, &item);
	ListView_SetItemText(hList, count, 1, phone);
}

int SelectItem(HWND hDlg, LPARAM lParam)
{
	LPNMLISTVIEW	nlv;
	HWND			hList;
	TCHAR			name[20], phone[20];
	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
	nlv = (LPNMLISTVIEW)lParam;
	ListView_GetItemText(hList, nlv->iItem, 0, name, 20);
	SetDlgItemText(hDlg, IDC_EDIT_NAME, name);
	ListView_GetItemText(hList, nlv->iItem, 1, phone, 20);
	SetDlgItemText(hDlg, IDC_EDIT_PHONE, phone);
	return(nlv->iItem);
}

void ModifyItem(HWND hDlg, int selection)
{
	static HWND		hList;
	TCHAR			name[20], phone[20];
	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
	GetDlgItemText(hDlg, IDC_EDIT_NAME, name, 20);
	GetDlgItemText(hDlg, IDC_EDIT_PHONE, phone, 20);
	if (_tcscmp(name, _T("")) == 0) return;
	ListView_SetItemText(hList, selection, 0, name);
	ListView_SetItemText(hList, selection, 1, phone);
	SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
	SetDlgItemText(hDlg, IDC_EDIT_PHONE, _T(""));
	return;
}

void DeleteItem(HWND hDlg, int selection)
{
	static HWND hList;
	hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
	ListView_DeleteItem(hList, selection);
	SetDlgItemText(hDlg, IDC_EDIT_NAME, _T(""));
	SetDlgItemText(hDlg, IDC_EDIT_PHONE, _T(""));
	return;
}

#define UNSELECTED -1
BOOL CALLBACK ListControl_Proc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR			hdr;
	static HWND			hList;
	static int		selection;

	switch (iMsg) {
	case WM_INITDIALOG:
		MakeColumn(hDlg);
		selection = UNSELECTED;
		return 1;
	case WM_NOTIFY:
		hdr = (LPNMHDR)lParam;
		hList = GetDlgItem(hDlg, IDC_LIST_MEMBER);
		if (hdr->hwndFrom == hList && hdr->code == LVN_ITEMCHANGING) selection = SelectItem(hDlg, lParam);
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON_INSERT:
			InsertData(hDlg);
			return 0;
		case IDC_BUTTON_MODIFY:
			if (selection == UNSELECTED) break;
			ModifyItem(hDlg, selection);
			selection = UNSELECTED;
			return 0;
		case IDC_BUTTON_DELETE:
			if (selection == UNSELECTED) break;
			DeleteItem(hDlg, selection);
			selection = UNSELECTED;
			return 0;
		case IDCLOSE:
			DestroyWindow(hDlg);
			hDlg = NULL;
			return 0;
		}
	}
	return 0;
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
	// <<

	// >> : sigong
	{
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hTransparentImage);
		bx = bitTransparent.bmWidth;
		by = bitTransparent.bmHeight;
		BitBlt(hMemDC, 100, 200, bx, by, hMemDC2, 0, 0, SRCCOPY);
		TransparentBlt(hMemDC, 200, 200, bx, by, hMemDC2, 0, 0, bx, by, RGB(255, 0, 255));

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}
	// <<

	// >> : animation
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

		bx = bitAni.bmWidth / 16;
		by = bitAni.bmHeight / 2;

		int xStart = curframe * bx;
		int yStart = 0;

		//StretchBlt(hReverseMemDC, bx - 1, 0, -bx, by, hMemDC2, xStart, yStart, bx, by, SRCCOPY);

		TransparentBlt(hMemDC, 200, 400, bx * 2, by * 2,
			hMemDC2, xStart, yStart, bx, by, RGB(255, 0, 255));

		//bx = SPRITE_SIZE_X;
		//by = SPRITE_SIZE_Y;
		//switch (flag)
		//{
		//case 1:
		//	Start_Pos_X = (curframe + 6) * SPRITE_SIZE_X;
		//	Start_Pos_Y = 1 * SPRITE_SIZE_Y;
		//	break;
		//case 2:
		//	Start_Pos_X = curframe * SPRITE_SIZE_X;
		//	Start_Pos_Y = 0 * SPRITE_SIZE_Y;
		//	break;
		//case 3:
		//	Start_Pos_X = (curframe + 6) * SPRITE_SIZE_X;
		//	Start_Pos_Y = 0 * SPRITE_SIZE_Y;
		//	break;
		//case 4:
		//	Start_Pos_X = curframe * SPRITE_SIZE_X;
		//	Start_Pos_Y = 1 * SPRITE_SIZE_Y;
		//	break;
		//case 6:
		//	Start_Pos_X = curframe * SPRITE_SIZE_X;
		//	Start_Pos_Y = 2 * SPRITE_SIZE_Y;
		//	break;
		//case 7:
		//	Start_Pos_X = (curframe + 6) * SPRITE_SIZE_X;
		//	Start_Pos_Y = 2 * SPRITE_SIZE_Y;
		//	break;
		//case 8:
		//	Start_Pos_X = curframe * SPRITE_SIZE_X;
		//	Start_Pos_Y = 3 * SPRITE_SIZE_Y;
		//	break;
		//case 9:
		//	Start_Pos_X = (curframe + 6) * SPRITE_SIZE_X;
		//	Start_Pos_Y = 3 * SPRITE_SIZE_Y;
		//	break;
		//default:
		//	break;
		//}
		//TransparentBlt(hMemDC, AniPos.x, AniPos.y, 2*bx, 2*by, hMemDC2,
		//	Start_Pos_X, Start_Pos_Y, bx, by, RGB(255, 255, 255));

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}
	// <<

	Gdi_Draw(hMemDC);

	BitBlt(hdc, 0, 0, rectView.right, rectView.bottom,
		hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteDC(hMemDC);
}

// >> GDI+
void Gdi_Init()
{
	GdiplusStartupInput gpsi;
	GdiplusStartup(&g_GdiToken, &gpsi, NULL);
}

void Gdi_Draw(HDC hdc)
{
	OnGdi_Paint(hdc);
}

void Gdi_End()
{
	GdiplusShutdown(g_GdiToken);
}

void OnGdi_Paint(HDC hdc)
{
	Graphics graphics(hdc);

	// text print
	SolidBrush brush(Color(255, 255, 0, 0));
	FontFamily fontFamily(L"Times New Roman");
	Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	PointF pointF(10.0f, 20.0f);
	graphics.DrawString(L"Hello GDI+", -1, &font, pointF, &brush);

	// line
	Pen pen(Color(255, 0, 255, 255)); // 불투명도, R, G, B
	graphics.DrawLine(&pen, 0, 0, 200, 100);

	// image
	Image img((WCHAR*)L"images/sigong.png");
	int w = img.GetWidth();
	int h = img.GetHeight();
	graphics.DrawImage(&img, 100, 100, w, h);

	Image* pImg = nullptr;
	pImg = Image::FromFile(((WCHAR*)L"images/sigong.png"));

	Gdiplus::Matrix mat;
	static int rot = 0;
	int xPos = 200;
	int yPos = 100;
	mat.RotateAt(rot % 360, Gdiplus::PointF(xPos + (float)w / 2, yPos + (float)h / 2));
	graphics.SetTransform(&mat);
	graphics.DrawImage(pImg, 200, 100, w, h);

	mat.RotateAt(-(rot % 360), Gdiplus::PointF(xPos + (float)w / 2, yPos + (float)h / 2));
	graphics.SetTransform(&mat);
	rot += 100;

	// >> : 배경 제거
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(240, 0, 240), Color(255, 10, 255));
	xPos = 300;
	graphics.DrawImage(pImg, Rect(xPos, yPos, w, h),
		0, 0, w, h, UnitPixel, &imgAttr);
	// <<

	// >> :
	brush.SetColor(Color(128, 255, 0, 0));
	graphics.FillRectangle(&brush, 400, 300, 200, 300);
	// <<

	// >> : 투명화
	static REAL transparency = 0.5f;
	transparency += 0.1f;
	if (transparency > 1.0f) transparency = 0.0f;
	ColorMatrix colorMatrix =
	{
		//	  R		G	  B     A
			1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, transparency, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	imgAttr.SetColorMatrix(&colorMatrix);
	xPos = 400;
	graphics.DrawImage(pImg, Rect(xPos, yPos, w, h),
		0, 0, w, h, UnitPixel, &imgAttr);
	
	colorMatrix =
	{
		//	  R		G	  B     A
			0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
			0.6f, 0.6f, 0.6f, 0.0f, 0.0f,
			0.1f, 0.1f, 0.1f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	imgAttr.SetColorMatrix(&colorMatrix);
	xPos = 500;
	graphics.DrawImage(pImg, Rect(xPos, yPos, w, h),
		0, 0, w, h, UnitPixel, &imgAttr);
	// <<

	if (pImg) delete pImg;
}
// <<

void OutFromFile(TCHAR filename[], HWND hWnd) {
	FILE *fPtr;
	HDC hdc;
	int line;
	TCHAR buffer[500];
	line = 0;
	hdc = GetDC(hWnd);
#ifdef _UNICODE
	_tfopen_s(&fPtr, filename, _T("r, ccs = UNICODE"));
#else
	_tfopen_s(&fPtr, filename, _T("r"));
#endif
	while (_fgetts(buffer, 100, fPtr) != NULL) {
		if (buffer[_tcslen(buffer) - 1] == _T('\n'))
			buffer[_tcslen(buffer) - 1] = NULL;
		TextOut(hdc, 0, line * 20, buffer, _tcslen(buffer));
		line++;
	}
	fclose(fPtr);
	ReleaseDC(hWnd, hdc);
}