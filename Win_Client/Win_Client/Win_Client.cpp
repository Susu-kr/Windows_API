// WinClient.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "framework.h"
#include "Win_Client.h"

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
HWND	hDlg = NULL;

TCHAR msg[1024];
TCHAR name[1024];
char buffer[1024];

int size, msgLen, msgCount;

//int Win_Client();

int Win_Client_Init(HWND hWnd);
void Win_Client_Close();
void ReadMessage(TCHAR *msg, char * buffer, HWND &msgList);
void MsgToBuffer();
BOOL CALLBACK ChatNICKNAMEProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ChatClientProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	//return Win_Client();

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_WINCLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINCLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINCLIENT);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 480, 340, nullptr, nullptr, hInstance, nullptr);

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
		if (!IsWindow(hDlg))
		{
			hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CHAT_NICKNAME), hWnd, ChatNICKNAMEProc);
			ShowWindow(hDlg, SW_SHOW);
		}

		return 0;
	}
	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
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

//	case WM_ASYNC:
//	{
//		switch ((lParam))
//		{
//		case FD_READ:
//		{
//			msgLen = recv(s, buffer, 100, 0);
//			buffer[msgLen] = NULL;
//
//#ifdef _UNICODE
//
//			msgLen = MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), NULL, NULL);
//			MultiByteToWideChar(CP_ACP, 0, buffer, strlen(buffer), msg, msgLen);
//			msg[msgLen] = NULL;
//#else
//
//			strcpy(bufferm str);
//
//#endif
//			InvalidateRect(hWnd, NULL, TRUE);
//		}
//		break;
//		}
//	}
//	break;
//
//	case WM_CHAR:
//	{
//		if (wParam == VK_RETURN)
//		{
//			if (s == INVALID_SOCKET)
//				return 0;
//
//#ifdef _UNICODE
//
//			msgLen = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
//			WideCharToMultiByte(CP_ACP, 0, str, -1, buffer, msgLen, NULL, NULL);
//
//#else
//
//			strcpy(bufferm str);
//			msgLen = strlen(buffer);
//
//#endif
//
//			send(s, (LPSTR)buffer, msgLen + 1, 0);
//			msgCount = 0;
//
//			return 0;
//		}
//
//		str[msgCount++] = wParam;
//		str[msgCount] = NULL;
//		InvalidateRect(hWnd, NULL, true);
//	}
//	break;
//
//	case WM_PAINT:
//	{
//		PAINTSTRUCT ps;
//		HDC hdc = BeginPaint(hWnd, &ps);
//
//		TextOut(hdc, 0, 0, str, _tcslen(str));
//
//		if (_tcscmp(msg, _T("")))
//			TextOut(hdc, 0, 30, msg, (int)_tcslen(msg));
//
//		EndPaint(hWnd, &ps);
//	}
//	break;

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

int Win_Client_Init(HWND hWnd)
{
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	s = socket(AF_INET, SOCK_STREAM, 0);

	addr.sin_family = AF_INET;
	addr.sin_port = 20;
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
		return 0;

	WSAAsyncSelect(s, hWnd, WM_ASYNC, FD_READ);

	return 1;
}

void Win_Client_Close()
{
	closesocket(s);
	WSACleanup();
}

void ReadMessage(TCHAR *msg, char *buffer, HWND &msgList)
{
	int msgLen = recv(s, buffer, 1024, 0);

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
		SendMessage(msgList, LB_ADDSTRING, 0, (LPARAM)msg);
	}
}


void MsgToBuffer()
{
	int msgLen;

#ifdef _UNICODE

	msgLen = WideCharToMultiByte(CP_ACP, 0, msg, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, msg, -1, buffer, msgLen, NULL, NULL);

#else
	strcpy(buffer, msg);
	msgLen = strlen(buffer);
#endif

	buffer[msgLen] = NULL;
}

BOOL CALLBACK ChatNICKNAMEProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND btnConnect;
	static HWND msgEdit;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		btnConnect = GetDlgItem(hWnd, IDC_BUTTON_CONNECT);
		msgEdit = GetDlgItem(hWnd, IDC_EDIT_MSG);
		return 1;
	}
	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDC_BUTTON_CONNECT:
		{
			if (Win_Client_Init(hWnd))
			{
				GetDlgItemText(hDlg, IDC_EDIT_NICKNAME, name, 1024);
				EndDialog(hDlg, 0);

				return 0;
			}
		}
		break;
		}
	}
	return 0;
	}
}

BOOL CALLBACK ChatClientProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND msgList;
	static HWND btnDisconnect;
	static HWND btnSend;
	static HWND msgEdit;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		msgList = GetDlgItem(hWnd, IDC_LIST_MSG);
		btnDisconnect = GetDlgItem(hWnd, IDC_BUTTON_DISCONNECT);
		btnSend = GetDlgItem(hWnd, IDC_BUTTON_SEND);
		msgEdit = GetDlgItem(hWnd, IDC_EDIT_MSG);

		return 1;
	}
	break;

	case WM_ASYNC:
	{
		switch (lParam)
		{
		case FD_READ:
		{
			ReadMessage(msg, buffer, msgList);
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
		case IDC_BUTTON_DISCONNECT:
		{
			Win_Client_Close();
			_tcscat_s(name, 1024, _T("님이 연결 종료하셨습니다."));
			SendMessage(msgList, LB_ADDSTRING, 0, (LPARAM)name);
			EndDialog(hDlg, 0);
			return 0;
		}
		break;

		case IDC_BUTTON_SEND:
		{
			GetWindowText(msgEdit, msg, sizeof(msg) / sizeof(TCHAR));
			SetWindowText(msgEdit, _T(""));
			MsgToBuffer();
			send(s, (LPSTR)buffer, strlen(buffer) + 1, 0);

			return 0;
		}
		break;
		}
	}
	break;
	}

	return 0;
}
