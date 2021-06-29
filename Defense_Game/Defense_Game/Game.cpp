#include "Game.h"

void GAME::Lobby(HDC hdc, RECT & R, TCHAR *str, SIZE temp)
{
	TextOut(hdc, R.right / 2 - 100, R.bottom / 2 - 100, _T("ID를 입력해주세요"), _tcslen(_T("ID를 입력해주세요")));
	Rectangle(hdc, R.right / 2 - 120, R.bottom / 2 - 80, R.right / 2 + 50, R.bottom / 2 - 50);
	RECT text = { R.right / 2 - 100, R.bottom / 2 - 75, R.right / 2 + 50, R.bottom / 2 - 55 };
	GetTextExtentPoint(hdc, str, _tcslen(str), &temp);
	TextOut(hdc, text.left - 10, text.top, str, _tcslen(str));
	SetCaretPos(temp.cx + text.left - 10, text.top);
}

void GAME::InGame()
{
	
}

void GAME::EndGame(HDC hdc, RECT & R, TCHAR * P)
{
	score = 10;
	TCHAR temp[1024];
	TextOut(hdc, R.right / 2 - 100, R.bottom / 2 - 100, _T("현재 플레이어 기록"), _tcslen(_T("현재 플레이어 기록")));
	TextOut(hdc, R.right / 2 - (75 + _tcslen(P)), R.bottom / 2 - 80, P, _tcslen(P));
}
