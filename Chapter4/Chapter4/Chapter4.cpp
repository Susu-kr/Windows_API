// Chapter4.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Chapter4.h"
#include "Object.h"
#include "DLL.h"
#include <cmath>
#include <time.h>
#include <commdlg.h>

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
	static List			object;
	static int			flag;

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
			GetClientRect(hWnd, &rcRect);
			SetTimer(hWnd, 1, 10, NULL);
		}
		break;
	case WM_SIZE:
		{
			GetClientRect(hWnd, &rcRect);
			InvalidateRect(hWnd, NULL, true);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			int num = rand() % 10;
			switch (num % 3)
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
		}
		break;
	case WM_TIMER:
	{
		switch (wParam) {
		case 1: {
			for (int i = 0; i < object.ListSize(); i++) {
				object.OBINDEX(i)->Update();
			}
			for (int i = 0; i < object.ListSize(); i++) {
				object.OBINDEX(i)->Collision(rcRect);
			}
			for (int i = 0; i < object.ListSize(); i++) {
				for (int j = i + 1; j < object.ListSize(); j++) {
					if (flag == 0) object.OBINDEX(i)->Collision(object.OBINDEX(j), NORMAL);
					else if (flag == 1) {
						object.OBINDEX(i)->Collision(object.OBINDEX(j), BIGGER);
						if (!object.OBINDEX(i)->GetActive()) object.removeIndex(i);
						else if (!object.OBINDEX(j)->GetActive()) object.removeIndex(j);
					}
					else if (flag == 2) {
						object.OBINDEX(i)->Collision(object.OBINDEX(j), SMALLER);
						if (!object.OBINDEX(i)->GetActive()) {
							switch (object.OBINDEX(j)->GetType()) {
							case CIRCLE: {
								object.insertBack(new CCircle(object.OBINDEX(i)->GetCenter().x, object.OBINDEX(i)->GetCenter().y
									, object.OBINDEX(i)->GetDirection().x, object.OBINDEX(i)->GetDirection().y, object.OBINDEX(j)->GetSize()));
							}
								break;
							case RECTANGLE: {
								object.insertBack(new CRect(object.OBINDEX(i)->GetCenter().x, object.OBINDEX(i)->GetCenter().y
									, object.OBINDEX(i)->GetDirection().x, object.OBINDEX(i)->GetDirection().y, object.OBINDEX(j)->GetSize()));
							}
								break;
							case STAR: {
								object.insertBack(new CStar(object.OBINDEX(i)->GetCenter().x, object.OBINDEX(i)->GetCenter().y
									, object.OBINDEX(i)->GetDirection().x, object.OBINDEX(i)->GetDirection().y, object.OBINDEX(j)->GetSize()));
							}
								break;
							}
							object.removeIndex(i);
						}
						else if (!object.OBINDEX(j)->GetActive()) {
							switch (object.OBINDEX(i)->GetType()) {
							case CIRCLE: {
								object.insertBack(new CCircle(object.OBINDEX(j)->GetCenter().x, object.OBINDEX(j)->GetCenter().y
									, object.OBINDEX(j)->GetDirection().x, object.OBINDEX(j)->GetDirection().y, object.OBINDEX(i)->GetSize()));
							}
								break;
							case RECTANGLE: {
								object.insertBack(new CRect(object.OBINDEX(j)->GetCenter().x, object.OBINDEX(j)->GetCenter().y
									, object.OBINDEX(j)->GetDirection().x, object.OBINDEX(j)->GetDirection().y, object.OBINDEX(i)->GetSize()));
							}
								break;
							case STAR: {
								object.insertBack(new CStar(object.OBINDEX(j)->GetCenter().x, object.OBINDEX(j)->GetCenter().y
									, object.OBINDEX(j)->GetDirection().x, object.OBINDEX(j)->GetDirection().y, object.OBINDEX(i)->GetSize()));
							}
								break;
							}
							object.removeIndex(j);
						}
					}
				}
			}
			InvalidateRect(hWnd, NULL, true);
		}
				break;
		}
	}
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
				MessageBox(hWnd, _T("기본설정으로 작동합니다."), _T("기능 선택"), MB_OK);
				flag = 0;
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
				flag = 1;
			}
				break;
			case ID_FUNC3:
				flag = 2;
				break;
			
			case ID_FUNC4:
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
			for (int i = 0; i < object.ListSize(); i++) {
				object.OBINDEX(i)->Draw(hdc);
			}
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
		for (int i = 0; i < object.ListSize(); i++) {
			object.removeFront();
		}
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