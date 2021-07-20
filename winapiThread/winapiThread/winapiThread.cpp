#include <iostream>
#include <process.h> // << : 1 thread 
#include <windows.h>

using namespace std;

int Value = 0;
int ValuePlusOne = 1;
int Count = 0;

CRITICAL_SECTION cs;

DWORD WINAPI ThFunc1(LPVOID lpParam);
DWORD WINAPI ThFunc2(LPVOID lpParam);
unsigned __stdcall ThFunc3(LPVOID lParam);
unsigned __stdcall ThFunc4(LPVOID lParam);

//1. CreateThread
//2. _beginthread, _beginthreadex
//3. AfxbeginThread <-MFC

int main()
{
	InitializeCriticalSection(&cs);
	DWORD dwThID1, dwThID2;
	HANDLE hThreads[2];

	unsigned long ulStackSize = 0;

	dwThID1 = 0;
	dwThID2 = 0;
	hThreads[0] = NULL;
	hThreads[1] = NULL;

	//hThreads[0] = CreateThread(NULL, ulStackSize, ThFunc1, NULL,
	//	CREATE_SUSPENDED, &dwThID1);
	//hThreads[1] = CreateThread(NULL, ulStackSize, ThFunc2, NULL,
	//	CREATE_SUSPENDED, &dwThID2);

	hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, (unsigned(__stdcall *)(void *)) ThFunc3,
		NULL, 0, (unsigned*)&dwThID1);

	hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, (unsigned(__stdcall *)(void *)) ThFunc4,
		NULL, 0, (unsigned*)&dwThID2);

	if ((hThreads[0] == 0) | (hThreads[1] == 0)) exit(1);


	ResumeThread(hThreads[0]);
	ResumeThread(hThreads[1]);

	ExitThread(dwThID1);
	DWORD dwExitCode = 0;
	GetExitCodeThread(hThreads[0], &dwExitCode);
	if (dwExitCode == STILL_ACTIVE)
	{
		CloseHandle(hThreads[0]);
		TerminateThread(hThreads[0], dwExitCode);
	}


	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

	CloseHandle(hThreads[0]);
	CloseHandle(hThreads[1]);

	DeleteCriticalSection(&cs);
}



DWORD WINAPI ThFunc1(LPVOID lParam)
{
	while (1)
	{
		EnterCriticalSection(&cs);

		Value = rand() % 1000;
		ValuePlusOne = Value + 1;

		LeaveCriticalSection(&cs);
	}
	return 0;
}

DWORD WINAPI ThFunc2(LPVOID lParam)
{
	while (1)
	{
		EnterCriticalSection(&cs);

		if (Value + 1 != ValuePlusOne)
			cout << ++Count << " -> " << Value << " : " << ValuePlusOne << endl;
	
		LeaveCriticalSection(&cs);
	}
	return 0;
}

unsigned __stdcall ThFunc3(LPVOID lParam)
{
	while (1)
	{
		EnterCriticalSection(&cs);

		Value = rand() % 1000;
		ValuePlusOne = Value + 1;

		LeaveCriticalSection(&cs);
	}
	return 0;
}

unsigned __stdcall ThFunc4(LPVOID lParam)
{
	while (1)
	{
		EnterCriticalSection(&cs);

		if (Value + 1 != ValuePlusOne)
			cout << ++Count << " -> " << Value << " : " << ValuePlusOne << endl;

		LeaveCriticalSection(&cs);
	}
	return 0;
}