#include "Game.h"

GAME::~GAME()
{
	for (int i = 0; i < wall.size(); i++)
		delete wall[i];
	for (int i = 0; i < bullet.size(); i++)
		delete bullet[i];
	for (int i = 0; i < enemy.size(); i++)
		delete enemy[i];
	delete cannon;
}

void GAME::LOBBY(HDC & hdc, RECT & R, SIZE & size)
{
	TextOut(hdc, R.right / 2 - 100, R.bottom / 2 - 100, _T("ID를 입력해주세요"), _tcslen(_T("ID를 입력해주세요")));
	Rectangle(hdc, R.right / 2 - 120, R.bottom / 2 - 80, R.right / 2 + 50, R.bottom / 2 - 50);
	RECT text = { R.right / 2 - 100, R.bottom / 2 - 75, R.right / 2 + 50, R.bottom / 2 - 55 };
	DrawText(hdc, ID, _tcslen(ID), &text, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	GetTextExtentPoint(hdc, ID, _tcslen(ID), &size);
	SetCaretPos(size.cx + text.left - 10, text.top);
}

void GAME::INGAME(HDC & hdc, RECT & R)
{
	RECT id_rect = { 50, 10, 200, 27 };
	DrawText(hdc, _T("ID: "), _tcslen(_T("ID: ")), &id_rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	id_rect.left += 25;
	DrawText(hdc, ID, _tcslen(ID), &id_rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
	id_rect.left += 250;
	id_rect.right += 250;

	TCHAR SCORE[10] = { 0 };
	TCHAR temp[100] = _T("Score: ");
	_itot_s(Score, SCORE, _countof(SCORE), 10);
	int j = 7;
	for (int i = 0; i < 6 - _tcslen(SCORE); i++)
	{
		temp[7 + i] = _T('0');
		j = 7 + i;

	}
	for (int i = 0; i < _tcslen(SCORE); i++)
		temp[j++] = SCORE[i];
	DrawText(hdc, temp, _tcslen(temp), &id_rect, DT_SINGLELINE | DT_VCENTER | DT_LEFT);

	Draw_Cannon(hdc);
	Draw_Wall(hdc);
	for (int i = 0; i < bullet.size(); i++)
		Draw_Bullet(hdc, i);

	for (int i = 0; i < enemy.size(); i++)
		Draw_Enemy(hdc, i);
}

void GAME::ENDGAME(HDC & hdc, RECT & R, TCHAR name[][15], int * score)
{
	TCHAR SCORE[100] = { 0 };
	_itot_s(Score, SCORE, _countof(SCORE), 10);
	TextOut(hdc, R.right / 2 - 155, R.bottom / 2 - 20, _T("현재 플레이어 기록"), _tcslen(_T("현재 플레이어 기록")));
	TextOut(hdc, R.right / 2 - 10, R.bottom / 2 - 20, ID, _tcslen(ID));
	TextOut(hdc, R.right / 2 + 65, R.bottom / 2 - 20, SCORE, _tcslen(SCORE));

	memset(SCORE, 0, sizeof(SCORE));
	_itot_s(score[0], SCORE, _countof(SCORE), 10);
	TextOut(hdc, R.right / 2 - 155, R.bottom / 2 + 5, _T("    1위    "), _tcslen(_T("    1위    ")));
	TextOut(hdc, R.right / 2 - 10, R.bottom / 2 + 5, name[0], _tcslen(name[0]));
	TextOut(hdc, R.right / 2 + 65, R.bottom / 2 + 5, SCORE, _tcslen(SCORE));
	memset(SCORE, 0, sizeof(SCORE));
	_itot_s(score[1], SCORE, _countof(SCORE), 10);
	TextOut(hdc, R.right / 2 - 155, R.bottom / 2 + 30, _T("    2위    "), _tcslen(_T("    2위    ")));
	TextOut(hdc, R.right / 2 - 10, R.bottom / 2 + 30, name[1], _tcslen(name[1]));
	TextOut(hdc, R.right / 2 + 65, R.bottom / 2 + 30, SCORE, _tcslen(SCORE));
	memset(SCORE, 0, sizeof(SCORE));
	_itot_s(score[2], SCORE, _countof(SCORE), 10);
	TextOut(hdc, R.right / 2 - 155, R.bottom / 2 + 55, _T("    3위    "), _tcslen(_T("    3위    ")));
	TextOut(hdc, R.right / 2 - 10, R.bottom / 2 + 55, name[2], _tcslen(name[2]));
	TextOut(hdc, R.right / 2 + 65, R.bottom / 2 + 55, SCORE, _tcslen(SCORE));
	memset(SCORE, 0, sizeof(SCORE));
	_itot_s(score[3], SCORE, _countof(SCORE), 10);
	TextOut(hdc, R.right / 2 - 155, R.bottom / 2 + 80, _T("    4위    "), _tcslen(_T("    4위    ")));
	TextOut(hdc, R.right / 2 - 10, R.bottom / 2 + 80, name[3], _tcslen(name[3]));
	TextOut(hdc, R.right / 2 + 65, R.bottom / 2 + 80, SCORE, _tcslen(SCORE));
	memset(SCORE, 0, sizeof(SCORE));
	_itot_s(score[4], SCORE, _countof(SCORE), 10);
	TextOut(hdc, R.right / 2 - 155, R.bottom / 2 + 105, _T("    5위    "), _tcslen(_T("    5위    ")));
	TextOut(hdc, R.right / 2 - 10, R.bottom / 2 + 105, name[4], _tcslen(name[4]));
	TextOut(hdc, R.right / 2 + 65, R.bottom / 2 + 105, SCORE, _tcslen(SCORE));
}

void GAME::Update(RECT)
{

}

void GAME::Create_Cannon(RECT & R, FLOAT rad)
{
	FLOAT x = R.right / 2;
	FLOAT y = R.bottom - rad - 5;
	cannon = new PLAY(x, y, rad);
}

void GAME::Create_Wall(RECT & R, FLOAT w, FLOAT h)
{
	FLOAT x = w / 2;
	FLOAT y = (R.bottom - h / 2) - 5;
	Wall * temp;
	while (x < R.right) {
		temp = new Wall(x, y, w, h);
		wall.push_back(temp);
		x += w + 1;
	}
	HP = wall.size();
}

void GAME::Create_Enemy(RECT & R, FLOAT rad)
{
	srand(unsigned int(time(NULL)));
	Enemy * e = new Enemy(rand() % (R.right - (2 * rad)) + rad, rad, 0, 1, rad);
}

void GAME::Save(TCHAR(*name)[15], int * score)
{
	ifstream fin("score.txt", ios::in | ios::binary);

	for (int i = 0; i < 5; i++)
	{
		fin.read((char *)name[i], sizeof(TCHAR) * 15);
		fin.read((char *)&score[i], sizeof(int));
	}
	fin.close();
	TCHAR tt[15];
	_tcscpy_s(tt, _countof(tt), ID);
	int nt = Score;

	for (int i = 0; i < 5; i++)
	{
		if (score[i] < Score)
		{
			TCHAR temp[15] = { 0 };
			_tcscpy_s(temp, _countof(temp), name[i]);
			int int_temp = score[i];

			_tcscpy_s(name[i], _countof(name[i]), ID);
			score[i] = Score;

			_tcscpy_s(ID, _countof(ID), temp);
			Score = int_temp;
		}
	}

	_tcscpy_s(ID, _countof(ID), tt);
	Score = nt;

	ofstream fout("score.txt", ios::out | ios::binary);

	for (int i = 0; i < 5; i++)
	{
		fout.write((char *)name[i], sizeof(TCHAR) * 15);
		fout.write((char *)&score[i], sizeof(int));
	}

	fout.close();
}
