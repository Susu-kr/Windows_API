// WinClient.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "WinClient.h"

#define MAX_LOADSTRING 100

using namespace std;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

WSADATA wsadata;
SOCKET s;
SOCKADDR_IN addr = { 0 };

TCHAR msg[256], str[100];
char buffer[10];

int size, msgLen, msgCount;

//int Win_Client();

int Win_Client_Init(HWND hWnd);
void Win_Client_Close();

void SendMessageToServer();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	//return Win_Client();

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINCLIENT));

    MSG msg;

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

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
      return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_CREATE:
		{
			return Win_Client_Init(hWnd);
		}
		break;

		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);

			switch (wmId)
			{
				case IDM_ABOUT:
				{
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				}
				break;

				case IDM_EXIT:
				{
					DestroyWindow(hWnd);
				}
				break;

				default:
				{
					return DefWindowProc(hWnd, message, wParam, lParam);
				}
				break;
			}
		}
		break;

		case WM_ASYNC:
		{
			switch ((lParam))
			{
				case FD_READ:
				{
					msgLen = recv(s, buffer, 100, 0);
					buffer[msgLen] = NULL;

#ifdef _UNICODE

					msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
					MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
					msg[msgLen] = NULL;
#else

					strcpy(bufferm str);

#endif
					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
			}
		}
		break;

		case WM_CHAR:
		{
			if (wParam == VK_RETURN)
			{
				if (s == INVALID_SOCKET)
					return 0;

#ifdef _UNICODE

				msgLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
				WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, msgLen, NULL, NULL);

#else

				strcpy(bufferm str);
				msgLen = strlen(buffer);

#endif

				send(s, (LPSTR)buffer, msgLen + 1, 0);
				msgCount = 0;

				return 0;
			}

			str[msgCount++] = wParam;
			str[msgCount] = NULL;
			InvalidateRect(hWnd, NULL, true);
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			TextOut(hdc, 0, 0, str, _tcslen(str));

			if(_tcscmp(msg, _T("")))
				TextOut(hdc, 0, 30, msg, (int)_tcslen(msg));

			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY:
		{
			Win_Client_Close();

			PostQuitMessage(0);
		}
		break;

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

    }

    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);

    switch (message)
    {
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
		{
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
		}
		break;

    }

    return (INT_PTR)FALSE;
}

//int Win_Client()
//{
//	WSAStartup(MAKEWORD(2, 2), &wsadata);
//	s = socket(AF_INET, SOCK_STREAM, 0);
//
//	if (s == INVALID_SOCKET)
//	{
//		MessageBox(NULL, _T("Socket Failed"), _T("Error"), MB_OK);
//		return 0;
//	}
//	else
//	{
//		MessageBox(NULL, _T("Socket Success"), _T("Success"), MB_OK);
//	}
//
//	addr.sin_family = AF_INET;
//	addr.sin_port = 8080;
//	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
//
//	if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
//	{
//		MessageBox(NULL, _T("Connect Failed"), _T("Error"), MB_OK);
//		return 0;
//	}
//	else
//	{
//		MessageBox(NULL, _T("Connect Success"), _T("Success"), MB_OK);
//	}
//
//	send(s, "Hello, Server!", 14, 0);
//
//	closesocket(s);
//	WSACleanup();
//
//	return 0;
//}

int Win_Client_Init(HWND hWnd)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = 8080;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);

	if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == -1)
		return 0;

	return 1;
}

void Win_Client_Close()
{
	closesocket(s);
	WSACleanup();
}

void SendMessageToServer()
{
	send(s, "Hello, Server!", 14, 0);
}
