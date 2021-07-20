// WinServer.cpp : 애플리케이션에 대한 진입점을 정의합니다.
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "WinServer.h"

#define MAX_LOADSTRING 100

using namespace std;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

WSADATA wsadata;
SOCKET s, cs;
SOCKADDR_IN addr = { 0 };
SOCKADDR_IN c_addr;

TCHAR msg[256];

int size, msgLen;
char buffer[100];

list<SOCKET> socketList;

int Win_Server_Init(HWND hWnd);
int Win_Server_Close();

SOCKET AcceptSocket(HWND hWnd, SOCKET s, SOCKADDR_IN& c_addr);
void SendMessageToClient(char *buffer);
void ReadMessage(TCHAR* msg, char *buffer);

void Win_Server_Close_Client(SOCKET s);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINSERVER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINSERVER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINSERVER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINSERVER);
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
			return Win_Server_Init(hWnd);
		}
		break;

		case WM_ASYNC:
		{
			switch (lParam)
			{
				case FD_ACCEPT:
				{
					cs = AcceptSocket(hWnd, s, c_addr);
				}
				break;

				case FD_READ:
				{
					ReadMessage(msg, buffer);
					InvalidateRect(hWnd, NULL, true);
				}
				break;

				case FD_CLOSE:
				{
					Win_Server_Close_Client(wParam);
				}
				break;
			}
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

				}
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			if(_tcscmp(msg, _T("")))
				TextOut(hdc, 0, 30, msg, (int)_tcslen(msg));

			EndPaint(hWnd, &ps);
		}
		break;

		case WM_DESTROY:
		{
			Win_Server_Close();
			PostQuitMessage(0);
		}
		break;

		default:
		{

		}
		return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
		case WM_INITDIALOG:
		{
			return (INT_PTR)TRUE;
		}
		break;

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

//int Win_Server_Init()
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
//	if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)))
//	{
//		MessageBox(NULL, _T("Binding Failed"), _T("Error"), MB_OK);
//		return 0;
//	}
//	else
//	{
//		MessageBox(NULL, _T("Binding Success"), _T("Success"), MB_OK);
//	}
//
//	if (listen(s, 5) == -1)
//	{
//		MessageBox(NULL, _T("Listen Failed"), _T("Error"), MB_OK);
//		return 0;
//	}
//
//	do
//	{
//		SOCKADDR_IN c_addr;
//		int size = sizeof(c_addr);
//		SOCKET cs = accept(s, (LPSOCKADDR)&c_addr, &size);
//
//		char buffer[100];
//		int msgLen = recv(cs, buffer, 100, 0);
//		buffer[msgLen] = NULL;
//
//#ifdef _UNICODE
//
//		TCHAR wBuffer[100];
//		msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
//		MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), wBuffer, msgLen);
//		wBuffer[msgLen] = NULL;
//
//		_stprintf_s(msg, _T("Client Message : %s Sever Close?"), wBuffer);
//
//#else
//
//		_stprintf_s(msg, _T("Client Message : %s Sever Close?"), buffer);
//
//#endif
//
//	} while (MessageBox(NULL, msg, _T("Server message"), MB_YESNO) == IDNO);
//
//	return 0;
//}

int Win_Server_Init(HWND hWnd)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	s = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = 8080;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	bind(s, (LPSOCKADDR)&addr, sizeof(addr));

	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_ACCEPT);

	if (listen(s, 5) == -1)
		return 0;

	return 1;
}

int Win_Server_Close()
{
	closesocket(s);
	WSACleanup();
	return 0;
}

SOCKET AcceptSocket(HWND hWnd, SOCKET s, SOCKADDR_IN & c_addr)
{
	SOCKET cs;
	int size;
	size = sizeof(c_addr);

	cs = accept(s, (LPSOCKADDR)&c_addr, &size);

	WSAAsyncSelect(cs, hWnd, WM_ASYNC, FD_READ | FD_CLOSE);

	socketList.push_back(cs);

	return cs;
}

void SendMessageToClient(char * buffer)
{
	for (list<SOCKET>::iterator it = socketList.begin(); it != socketList.end(); ++it)
	{
		SOCKET cs = (*it);
		send(cs, (LPSTR)buffer, strlen(buffer) + 1, 0);
	}
}

void ReadMessage(TCHAR * msg, char * buffer)
{
	for (list<SOCKET>::iterator it = socketList.begin(); it != socketList.end(); ++it)
	{
		SOCKET cs = (*it);

		int msgLen = recv(cs, buffer, 100, 0);

		if (msgLen > 0)
		{
			buffer[msgLen] = NULL;

#ifdef _UNICODE

			msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
			msg[msgLen] = NULL;

#else

			strcpy(msg, buffer);

#endif

			SendMessageToClient(buffer);
		}
	}
}

void Win_Server_Close_Client(SOCKET socket)
{
	for (list<SOCKET>::iterator it = socketList.begin(); it != socketList.end(); ++it)
	{
		SOCKET cs = (*it);

		if (cs == socket)
		{
			closesocket(cs);
			it = socketList.erase(it);
			break;
		}
	}
}
