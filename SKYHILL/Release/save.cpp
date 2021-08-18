#include "GameManager.h"

// ����� �� �ΰ�� 
// ����� �� ũ�� : ���� ��� 140, 205 + 1�� 1010, 205 + ������ ��� 130, 205
// �� 1ĭ�� ũ�� : 336, 210, ��ܹ� ũ�� : 305, 210, �� ���� �� ���� = 30
// 1���� ũ�� : ���� ��� 137, 210 + ���ʹ� 336, 210 + �� 18, 210 + ��ܹ� ũ�� 307, 202 + �� 9, 202 + �����ʹ� 337, 202 + ������ ��� 130, 202
Rect rooms[] = { Rect(135, 466, 355, 215), Rect(490, 466, 315, 215), Rect(805, 466, 355, 215) // �Ʒ��� ����, ��ܹ�, �Ʒ��� ������
			   , Rect(135, 228, 355, 215), Rect(490, 228, 315, 215), Rect(805, 228, 355, 215) // �߰��� ����, ��ܹ�, �߰��� ������
			   , Rect(135, 0, 355, 215), Rect(490, 0, 315, 215), Rect(805, 0, 355, 215) }; // ���� ���� , ��ܹ�, ���� ������

void GameManager::Init()
{
	GDI_Init();
	string BG_path = "Sound/rain.wav";
	string EFF_path = "Sound/click.wav";
	sound.CreateBGSound(1, &BG_path);
	sound.CreateEFFSound(1, &EFF_path);
	loadImage(state);
	passive[0] = true;
	active[0] = true;
	sound.PlaySoundBG(0);
}

void GameManager::GameReset()
{
}

void GameManager::Update()
{
	if (state != INGAME) return;
	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;

	if (newTime - oldTime < 50) return;
	oldTime = newTime;
	sound.Update();
}

void GameManager::DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;
	//int bx, by;
	HDC hMemDC2;
	HBITMAP hOldBitmap2;

	hMemDC = CreateCompatibleDC(hdc);
	if (!hMemDC)
	{
		MessageBox(hWnd, _T("CreateCompatibleDC faild"),
			_T("Error"), MB_OK);
		return;
	}

	if (hDoubleBufferImage == NULL)
	{
		hDoubleBufferImage = CreateCompatibleBitmap(hdc, screenRect.right, screenRect.bottom);
	}

	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage);
	GDI_Draw(hMemDC);

	BitBlt(hdc, 0, 0, screenRect.right, screenRect.bottom, hMemDC, 0, 0, SRCCOPY);
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hMemDC);

}

void GameManager::Transition(int _state)
{
	sound.StopSoundBG(0);
	state = _state;
	loadImage(state);
	GameReset();
}

void GameManager::GDI_Init()
{
	GdiplusStartupInput gpsi;
	GdiplusStartup(&g_GdiToken, &gpsi, NULL);
}

void GameManager::GDI_Draw(HDC hdc)
{
	Graphics graphics(hdc);

	graphics.SetInterpolationMode(InterpolationModeHighQuality);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	switch (state)
	{
	case START:
		DrawStartScreen(graphics);
		break;
	case INGAME:
		DrawGameScreen(graphics);
		break;
	case END:
		DrawEndScreen(graphics);
		break;
	case CLEAR:
		DrawClearScreen(graphics);
		break;
	case OVER:
		DrawOverScreen(graphics);
		break;
	}
}

void GameManager::GDI_End()
{
	for (int i = 0; i < 10; i++)
		if (img[i]) delete img[i];
	GdiplusShutdown(g_GdiToken);
}

bool GameManager::Click(float x, float y)
{
	sound.PlaySoundEffect(0);
	switch (state)
	{
	case START:
	{
		if (!chk_op && !chk_game && !chk_ability)
		{
			// �ɼǹ�ư Ŭ��
			if (x >= screenRect.right - 225 && x < screenRect.right - 175
				&& y >= screenRect.bottom - 100 && y < screenRect.bottom - 50)
			{
				chk_op = true;
			}
			// ���ӽ��۹�ư Ŭ��
			else if (x >= screenRect.right - 150 && x < screenRect.right - 100
				&& y >= screenRect.bottom - 100 && y < screenRect.bottom - 50)
			{
				chk_game = true;
			}
			// �������ư Ŭ��
			else if (x >= screenRect.right - 75 && x < screenRect.right - 25
				&& y >= screenRect.bottom - 100 && y < screenRect.bottom - 50)
			{
				return false;
			}
		}
		else if (chk_op && !chk_game && !chk_ability)
		{
			// �������ư Ŭ��(����)
			if (x >= screenRect.right / 2 - 180 && x < screenRect.right / 2 - 155
				&& y >= screenRect.bottom / 2 - 50 && y < screenRect.bottom / 2 - 25)
			{
				sound.volumeDown();
			}

			// �������ư Ŭ��(����)
			else if (x >= screenRect.right / 2 + 150 && x < screenRect.right / 2 + 175
				&& y >= screenRect.bottom / 2 - 50 && y < screenRect.bottom / 2 - 25)
			{
				sound.volumeUp();
			}

			// ȿ������ư Ŭ��(����)
			else if (x >= screenRect.right / 2 - 180 && x < screenRect.right / 2 - 155
				&& y >= screenRect.bottom / 2 && y < screenRect.bottom / 2 + 25)
			{
				sound.e_volumeDown();
			}
			// ȿ������ư Ŭ��(����)
			else if (x >= screenRect.right / 2 + 150 && x < screenRect.right / 2 + 175
				&& y >= screenRect.bottom / 2 && y < screenRect.bottom / 2 + 25)
			{
				sound.e_volumeUp();
			}
			else if (x >= screenRect.right / 2 - 70 && x < screenRect.right / 2 + 65
				&& y >= screenRect.bottom / 2 + 87 && y < screenRect.bottom / 2 + 116)
			{
				chk_op = false;
			}
			return true;
		}
		else if (!chk_op && chk_game && !chk_ability) // ���� ���̵� ���ôܰ�
		{
			//screenRect.right / 2 - 112, screenRect.bottom / 2 - 119

			// ���� ��ư Ŭ��
			if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
				&& y >= screenRect.bottom / 2 - 75 && y < screenRect.bottom / 2 - 51)
			{
				// ���̵��� ���� ����
				chk_ability = true;
				difficulty = 1;
				// 
			}
			// ���� ��ư Ŭ��
			else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
				&& y >= screenRect.bottom / 2 - 32 && y < screenRect.bottom / 2 - 8)
			{
				// ���̵��� ���� ����
				chk_ability = true;
				difficulty = 2;
			}
			// ����� ��ư Ŭ��
			else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
				&& y >= screenRect.bottom / 2 + 17 && y < screenRect.bottom / 2 + 41)
			{
				// ���̵��� ���� ����
				chk_ability = true;
				difficulty = 3;
			}
			// ��� ��ư Ŭ��
			else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
				&& y >= screenRect.bottom / 2 + 82 && y < screenRect.bottom / 2 + 106)
			{
				chk_game = false;
			}
		}
		else if (chk_ability)
		{
			// �÷��̾ ���� ���
			if (x >= screenRect.right / 2 - 133 && x < screenRect.right / 2 - 71
				&& y >= screenRect.bottom / 2 - 85 && y < screenRect.bottom / 2 - 23)
			{// �ɷ� ����
				for (int i = 0; i < 4; i++)
				{
					if (i == 0) passive[i] = true;
					else passive[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 67 && x < screenRect.right / 2 - 5
				&& y >= screenRect.bottom / 2 - 85 && y < screenRect.bottom / 2 - 23)
			{// ���� ����
				for (int i = 0; i < 4; i++)
				{
					if (i == 1) passive[i] = true;
					else passive[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 1 && x < screenRect.right / 2 + 61
				&& y >= screenRect.bottom / 2 - 85 && y < screenRect.bottom / 2 - 23)
			{// ���� ŰƮ
				for (int i = 0; i < 4; i++)
				{
					if (i == 2) passive[i] = true;
					else passive[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 + 65 && x < screenRect.right / 2 + 127
				&& y >= screenRect.bottom / 2 - 85 && y < screenRect.bottom / 2 - 23)
			{// ������ ��
				for (int i = 0; i < 4; i++)
				{
					if (i == 3) passive[i] = true;
					else passive[i] = false;
				}
			}
			// �ڵ����� ����
			else if (x >= screenRect.right / 2 - 133 && x < screenRect.right / 2 - 71
				&& y >= screenRect.bottom / 2 + 29 && y < screenRect.bottom / 2 + 91)
			{// ����
				for (int i = 0; i < 4; i++)
				{
					if (i == 0) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 67 && x < screenRect.right / 2 - 5
				&& y >= screenRect.bottom / 2 + 29 && y < screenRect.bottom / 2 + 91)

			{// ���� �¿��ϴ� ����
				for (int i = 0; i < 4; i++)
				{
					if (i == 1) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 1 && x < screenRect.right / 2 + 61
				&& y >= screenRect.bottom / 2 + 29 && y < screenRect.bottom / 2 + 91)
			{// ������â
				for (int i = 0; i < 4; i++)
				{
					if (i == 2) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 + 65 && x < screenRect.right / 2 + 127
				&& y >= screenRect.bottom / 2 + 29 && y < screenRect.bottom / 2 + 91)
			{// ��Ұ�����
				for (int i = 0; i < 4; i++)
				{
					if (i == 3) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 257 && x < screenRect.right / 2 - 56
				&& y >= screenRect.bottom / 2 + 153 && y < screenRect.bottom / 2 + 177)
			{ // �ݱ�
				chk_ability = false;
			}
			else if (x >= screenRect.right / 2 + 57 && x < screenRect.right / 2 + 255
				&& y >= screenRect.bottom / 2 + 153 && y < screenRect.bottom / 2 + 177)
			{ // ���ӽ���
				Transition(INGAME);
				floor = 30;
				room_chk[30][0] = 2;
				room_chk[30][1] = 2;
				room_chk[30][2] = 2;
			}
		}
	}
	break;
	case INGAME:
	{
		if (floor == 30)
		{
			// rooms[0] ~ [2] = �� �Ʒ��� -> ����� ���̹Ƿ� �Ʒ��θ� �̵� ����
			if (x >= rooms[0].GetLeft() && x < rooms[0].GetRight() && y >= rooms[0].GetTop() && y < rooms[0].GetBottom())
			{
				int cnt = 0;
				while (cnt < 3)
				{
					room_chk[floor][cnt] = 1;
					cnt++;
				}
				floor = 29;
				room_chk[floor][1] = 2;
				move_chk = true;
				downmove_chk = true;
			}
			else if (x >= rooms[1].GetLeft() && x < rooms[1].GetRight() && y >= rooms[1].GetTop() && y < rooms[1].GetBottom())
			{
				int cnt = 0;
				while (cnt < 3)
				{
					room_chk[floor][cnt] = 1;
					cnt++;
				}
				floor = 29;
				room_chk[floor][1] = 2;
				move_chk = true;
				downmove_chk = true;
			}
			else if (x >= rooms[2].GetLeft() && x < rooms[2].GetRight() && y >= rooms[2].GetTop() && y < rooms[2].GetBottom())
			{
				int cnt = 0;
				while (cnt < 3)
				{
					room_chk[floor][cnt] = 1;
					cnt++;
				}
				floor = 29;
				room_chk[floor][1] = 2;
				move_chk = true;
				downmove_chk = true;
			}
		}
		else if (floor < 30 && floor > 1)
		{
			// rooms[0] ~ [2] = �� �Ʒ��� rooms[3] ~ [5] = �߰� �� , rooms[6]~ [8] = �� ����
			if (x >= rooms[0].GetLeft() && x < rooms[0].GetRight() && y >= rooms[0].GetTop() && y < rooms[0].GetBottom())
			{

			}
			else if (x >= rooms[1].GetLeft() && x < rooms[1].GetRight() && y >= rooms[1].GetTop() && y < rooms[1].GetBottom())
			{
				int cnt = 0;
				while (cnt < 3)
				{
					if (room_chk[floor][cnt] == 2) room_chk[floor][cnt] = 1;
					cnt++;
				}
				floor--;
				room_chk[floor][1] = 2;
				move_chk = true;
				downmove_chk = true;
			}
			else if (x >= rooms[2].GetLeft() && x < rooms[2].GetRight() && y >= rooms[2].GetTop() && y < rooms[2].GetBottom())
			{

			}
			else if (x >= rooms[3].GetLeft() && x < rooms[3].GetRight() && y >= rooms[3].GetTop() && y < rooms[3].GetBottom())
			{

			}
			else if (x >= rooms[5].GetLeft() && x < rooms[5].GetRight() && y >= rooms[5].GetTop() && y < rooms[5].GetBottom())
			{

			}
			else if (x >= rooms[6].GetLeft() && x < rooms[6].GetRight() && y >= rooms[6].GetTop() && y < rooms[6].GetBottom())
			{

			}
			else if (x >= rooms[7].GetLeft() && x < rooms[7].GetRight() && y >= rooms[7].GetTop() && y < rooms[7].GetBottom())
			{

			}
			else if (x >= rooms[8].GetLeft() && x < rooms[8].GetRight() && y >= rooms[8].GetTop() && y < rooms[8].GetBottom())
			{

			}
		}
	}
	break;
	case END:
		break;
	case CLEAR:
		break;
	case OVER:
		break;
	}
	return true;
}

void GameManager::loadImage(int state)
{
	switch (state)
	{
	case START:
	{
		img[0] = Image::FromFile(L"images/Lobby/Lobby.png");
		img[1] = Image::FromFile(L"images/Lobby/Setting.png");
		img[2] = Image::FromFile(L"images/Lobby/Play.png");
		img[3] = Image::FromFile(L"images/Lobby/End.png");
		img[4] = Image::FromFile(L"images/Lobby/Option.png");
		img[5] = Image::FromFile(L"images/Lobby/Difficult.png");
		img[6] = Image::FromFile(L"images/Lobby/Ability2.png");
	}
	break;
	case INGAME:
	{
		img[0] = Image::FromFile(L"images/Map/Map.png");
		img[1] = Image::FromFile(L"images/Player/Player-UI.png");
		img[2] = Image::FromFile(L"images/Player/Player_Ani.png");
		img[3] = Image::FromFile(L"images/Player/Bag.png");

	}
	break;
	case END:
		break;
	case CLEAR:
		break;
	case OVER:
		break;
	}

	if (!img[0]) return;
}



void GameManager::DrawStartScreen(Graphics & graphics)
{

	static int count = 0;

	// ���� �κ� ȭ���� ������ ����Ǹ� ���� �κк��� �ݺ����
	if (count >= 22600) count = 4520;

	//

	// ��� ����
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(240, 0, 240), Color(255, 10, 255));

	int w = img[1]->GetWidth();
	int h = img[1]->GetHeight();
	graphics.DrawImage(img[0], Rect(0, 0, screenRect.right, screenRect.bottom), count, 0, 1120, 762, UnitPixel);
	graphics.DrawImage(img[1], Rect(screenRect.right - 225, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(img[2], Rect(screenRect.right - 150, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(img[3], Rect(screenRect.right - 75, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);

	// �ɼ� �������� ���� ���
	if (chk_op) {
		// >> : ��� ��Ӱ�
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// �ɼ� â ���
		int o_w = img[4]->GetWidth();
		int o_h = img[4]->GetHeight();
		graphics.DrawImage(img[4], Rect(screenRect.right / 2 - 188, screenRect.bottom / 2 - 116, o_w, o_h), 0, 0, o_w, o_h, UnitPixel);

		// �ؽ�Ʈ �Է�
		brush.SetColor(Color(255, 145, 141, 120));

		// ��Ʈ ����
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, 15, FontStyleBold, UnitPixel);

		// �����, ȿ���� �� ���
		TCHAR BG[20], EFF[20];
		wsprintf(BG, TEXT("����� : %d%%"), (int)(sound.getM_vol() * 100));
		wsprintf(EFF, TEXT("ȿ���� : %d%%"), (int)(sound.getE_vol() * 100));
		PointF pointF1(screenRect.right / 2 - 50.0f, screenRect.bottom / 2 - 45.0f);
		graphics.DrawString(BG, -1, &font, pointF1, &brush);
		PointF pointF2(screenRect.right / 2 - 50.0f, screenRect.bottom / 2 + 10.0f);
		graphics.DrawString(EFF, -1, &font, pointF2, &brush);
	}
	if (chk_game && !chk_ability)
	{
		// >> : ��� ��Ӱ�
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// ���̵� ���� â ���
		int d_w = img[5]->GetWidth();
		int d_h = img[5]->GetHeight();
		graphics.DrawImage(img[5], Rect(screenRect.right / 2 - 112, screenRect.bottom / 2 - 119, d_w, d_h), 0, 0, d_w, d_h, UnitPixel);

		// ��Ʈ ����
		brush.SetColor(Color(255, 145, 141, 120));
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, 15, FontStyleBold, UnitPixel);

		// ���̵� ���
		PointF pointF1(screenRect.right / 2 - 22.0f, screenRect.bottom / 2 - 72.0f);
		graphics.DrawString(L"�� ��", -1, &font, pointF1, &brush);
		PointF pointF2(screenRect.right / 2 - 22.0f, screenRect.bottom / 2 - 30.0f);
		graphics.DrawString(L"�� ��", -1, &font, pointF2, &brush);
		PointF pointF3(screenRect.right / 2 - 32.0f, screenRect.bottom / 2 + 22.0f);
		graphics.DrawString(L"�� �� ��", -1, &font, pointF3, &brush);
	}
	if (chk_ability)
	{
		// >> : ��� ��Ӱ�
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// �ɷ� ���� â ���
		int a_w = img[6]->GetWidth();
		int a_h = img[6]->GetHeight();
		graphics.DrawImage(img[6], Rect(screenRect.right / 2 - 423, screenRect.bottom / 2 - 199, a_w, a_h), 0, 0, a_w, a_h, UnitPixel);

		// ������ �ɷ� ǥ��
		Pen RedPen(Color(255, 255, 0, 10), 2.0f);
		int p_x = screenRect.right / 2 - 130;
		int p_y = screenRect.bottom / 2 - 98;
		int a_x = p_x;
		int a_y = screenRect.bottom / 2 + 38;
		brush.SetColor(Color(255, 145, 141, 120));
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, 15, FontStyleBold, UnitPixel);
		for (int i = 0; i < 4; i++)
		{
			if (passive[i] == true)
			{
				p_x += i * 69;
				graphics.DrawRectangle(&RedPen, p_x, p_y, 63, 62);
				TCHAR temp[100];
				if (i == 1)
				{
					wsprintf(temp, TEXT("���� óġ : 1ȸ ��� ������ ��ư�� �����. ��� ��, HP�� 50��ŭ ȸ�������ش�."));
				}
				else if (i == 2)
				{
					wsprintf(temp, TEXT("���� ŰƮ : 1ȸ ��� ������ ��ư�� �����. ��� �� ������� 100��ŭ ȸ�������ش�."));
				}
				else if (i == 3)
				{
					wsprintf(temp, TEXT("�� ���� �� : 150ȸ �̵� �� ���� ����. ��� �� 1000�� ����� ��븦 ����Ų��."));
				}
				if (i != 0) {
					PointF pointF1(screenRect.right / 2 - 300.0f, screenRect.bottom / 2 - 20.0f);
					graphics.DrawString(temp, -1, &font, pointF1, &brush);
				}

			}
			if (active[i])
			{
				a_x += i * 69;
				graphics.DrawRectangle(&RedPen, a_x, a_y, 63, 62);
				TCHAR temp[100];
				if (i == 1)
				{
					wsprintf(temp, TEXT("���� �¿� �ϴ� ���� : ���� Ÿ���� �� �⺻ ���� ������� 1 ~ 300%%�� ������ Ư��"));
				}
				else if (i == 2)
				{
					wsprintf(temp, TEXT("������â : ������ ��� Ȯ���� 50%% ����, 10ȸ �̵����� ������ �������� �Ҿ������ Ư��"));
				}
				else if (i == 3)
				{
					wsprintf(temp, TEXT("��Ұ����� : ���� �ִ� HP�� 60�̳�, 5�� ���� ������������ �ִ� HP�� 10�� �����Ѵ�."));
				}

				if (i != 0) {
					PointF pointF1(screenRect.right / 2 - 300.0f, screenRect.bottom / 2 + 122.0f);
					graphics.DrawString(temp, -1, &font, pointF1, &brush);
				}

			}
		}
	}
	count += 1130;
}

void GameManager::DrawGameScreen(Graphics & graphics)
{
	static int count = 0;
	//if (count < 30)
	//{
	//	SolidBrush brush(Color(255, 0, 0, 0));
	//	brush.SetColor(Color(128, 0, 0, 0));
	//	graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
	//}

	/*
		1. �÷��̾ �� - �Ϸ� ������ ���� ī�޶� ���� ���� o
		2. �÷��̾ �� - ��� ������ ���� ī�޶� ���� ���� x
		3. �÷��̾ �밢�������� Ŭ������ ���, ��, �ϰ� ���� ���� �� �� �� �� ����
		4. �� -> ĳ���� -> �� ������ �׷��ְ�, �湮���� ���� ���� ������ �簢������ ����������
		(�÷��̾ �̵��� �� ����� �������ų� ���� �������Ƿ� ��(��)�� ��ġ�� �°� �����簢�� �׷��ֱ�)
		5. ���� ��ġ�� ���� ������ �簢���� ����, �湮�ߴ� ���� ������ �簢���� ������

		=============||=============||=============
					 ||				||
					 ||				||
		  �� = �� ��	 ||		 ��		||   �� = �� ��
					 ||				||
					 ||				||
		=============||=============||=============
		=============||=============||=============
					 ||				||
					 ||				||
			��		 ||				||		��
					 ||				||
					 ||				||
		=============||=============||=============
		=============||=============||=============
					 ||				||
					 ||				||
		  �� = �� ��	 ||		 ��		||	�� = �� ��
					 ||				||
					 ||				||
		=============||=============||=============

	*/



	SolidBrush brush(Color(255, 0, 0, 0));


	static int m_h = 20;
	static int b_h = 0;

	Rect move_rooms[] = { Rect(135, 690 - b_h, 355, 230), Rect(490, 700 - b_h, 320, 215), Rect(805, 700 - b_h, 360, 230) // �Ʒ��� ����, ��ܹ�, �Ʒ��� ������
			 , Rect(135, 463 - b_h, 355, 230), Rect(490, 473 - b_h, 320, 215), Rect(805, 463 - b_h, 360, 230) // �߰��� ����, ��ܹ�, �߰��� ������
			 , Rect(135, 225 - b_h, 355, 230), Rect(490, 235 - b_h, 320, 215), Rect(805, 225 - b_h, 360, 230)
			 , Rect(135, 0 - b_h, 355, 230), Rect(490, 0 - b_h, 320, 215), Rect(805, 0 - b_h, 360, 230) }; // ���� ���� , ��ܹ�, ���� ������
	if (move_chk) // �÷��̾ ������
	{
		if (b_h >= 215) b_h = 0;
		if (downmove_chk)
		{
			static int p_w = 0, p_h = 320;
			static int w_h = 458;
			if (floor < 30 && floor > 1)
			{
				static int p_x = 35, p_x2 = 135;
				static int p_y = 0, p_y1 = 0;
				static int p_h2 = 640, p_h3 = 0;
				static bool left = false;
				// ���� �̹��� ��� : graphics.DrawImage(img, 0, 0, 100, 100);
				// y�� ��Ī ��� : graphics.DrawImage(img, 100, 0, -100, 100);
				// ���

				if (p_w >= 2210) p_w = 0;
				p_x += 5;
				if (p_x < 165)
				{
					graphics.DrawImage(img[0], Rect(0, 0, screenRect.right, screenRect.bottom), 0, m_h, 1280, 700, UnitPixel);
					int cnt = 0;
					for (int i = floor - 1; i <= floor + 1 && i < 30; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							if (room_chk[i][j] == 1) // �ѹ� �湮 �ߴ� ���̶��
							{
								brush.SetColor(Color(128, 0, 0, 0)); // �������� ���� �簢��
								graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
							}
							else if (room_chk[i][j] == 0)
							{
								brush.SetColor(Color(255, 0, 0, 0)); // ���� �簢��
								graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
							}
						}
						cnt++;
					}
					graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w + 170, p_h, -170, 318, UnitPixel);
				}
				else
				{
					graphics.DrawImage(img[0], Rect(0, 0, screenRect.right, screenRect.bottom), 0, m_h, 1280, 700, UnitPixel);
					if (!left)
					{
						graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x2, screenRect.bottom / 2 + 110 - p_y, 90, p_y1), p_w, p_h2, 170, p_h3, UnitPixel);
						p_x2 -= 3;
						if (p_x2 <= 35) left = true;
					}
					else if (left)
					{
						graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x2, screenRect.bottom / 2 + 110 - p_y, 90, p_y1), p_w + 170, p_h2, -170, p_h3, UnitPixel);
						p_x2 += 3;
						if (p_x2 >= 156) left = false;
					}
					int cnt = 0;
					for (int i = floor - 1; i <= floor + 2 && i <= 30; i++)
					{
						for (int j = 0; j < 3; j++)
						{
							if (room_chk[i][j] == 1) // �ѹ� �湮 �ߴ� ���̶��
							{
								brush.SetColor(Color(128, 0, 0, 0)); // �������� ���� �簢��
								graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
							}
							else if (room_chk[i][j] == 0)
							{
								brush.SetColor(Color(255, 0, 0, 0)); // ���� �簢��
								graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
							}
						}
						cnt++;
					}
					Rect walls[] = { Rect(135, w_h, 1050, 25), Rect(799, w_h, 360, 680 - w_h) , Rect(495, 700 - b_h, 305, b_h) };
					brush.SetColor(Color(255, 0, 0, 0));
					graphics.FillRectangles(&brush, walls, 4);

					p_h2 -= 8;
					p_h3 += 8;
					p_y += 3;
					if (p_y1 < 120) p_y1 += 3;
					if (p_y >= 130) p_y = 130;;
					if (p_h2 <= 320)
					{
						p_h2 = 320;
						p_h3 = 320;
					}
					m_h += 3;
					w_h -= 3;
					b_h += 3;
				}
				p_w += 170;
				// ��� ��������
				if (m_h >= 248 + (29 - floor) * 231)
				{
					move_chk = false;
					p_x2 = 135;
					left = false;
					p_y = 0;
					p_y1 = 0;
					p_h2 = 640;
					p_h3 = 0;
					w_h = 458;
				}
			}
		}
		else if (upmove_chk)
		{
			if (floor == 30)
			{

			}
			else if (floor < 30 && floor > 1)
			{

			}
		}
	}
	else  // �÷��̾ ����
	{
		static int p_w = 0, p_h = 0;
		b_h = 230;
		if (p_w >= 2210)
		{
			p_w = 0;
		}
		if (floor == 30)
		{
			graphics.DrawImage(img[0], Rect(0, 0, screenRect.right, screenRect.bottom), 0, m_h, 1280, 700, UnitPixel);
			graphics.DrawImage(img[2], Rect(screenRect.right / 2 - 35, screenRect.bottom / 2 - 30, 90, 120), p_w, p_h, 170, 318, UnitPixel);
			// ������ �簢������ �湮�� �� �湮���� ������ �׸���.
			int cnt = 0;
			for (int i = floor - 1; i <= floor + 1 && i < 30; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (room_chk[i][j] == 1) // �ѹ� �湮 �ߴ� ���̶��
					{
						brush.SetColor(Color(128, 0, 0, 0)); // �������� ���� �簢��
						graphics.FillRectangle(&brush, rooms[cnt * 3 + j]);
					}
					else if (room_chk[i][j] == 0)
					{
						brush.SetColor(Color(255, 0, 0, 0)); // ���� �簢��
						graphics.FillRectangle(&brush, rooms[cnt * 3 + j]);
					}
				}
				cnt++;
			}
		}
		// 29�� ����
		else if (floor < 30 && floor > 1)
		{
			graphics.DrawImage(img[0], Rect(0, 0, screenRect.right, screenRect.bottom), 0, m_h, 1280, 700, UnitPixel);
			graphics.DrawImage(img[2], Rect(screenRect.right / 2 - 135, screenRect.bottom / 2 - 20, 90, 120), p_w, p_h, 170, 318, UnitPixel);
			int cnt = 0;
			for (int i = floor - 1; i <= floor + 1 && i <= 30; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (room_chk[i][j] == 1) // �ѹ� �湮 �ߴ� ���̶��
					{
						brush.SetColor(Color(128, 0, 0, 0)); // �������� ���� �簢��
						graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
					}
					else if (room_chk[i][j] == 0)
					{
						brush.SetColor(Color(255, 0, 0, 0)); // ���� �簢��
						graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
					}
				}
				cnt++;
			}
		}
		// 1���� ���
		else
		{

		}

		p_w += 170;


	}




	// UI �׸���
	graphics.DrawImage(img[1], Rect(10, 10, img[1]->GetWidth(), img[1]->GetHeight()), 0, 0, img[1]->GetWidth(), img[1]->GetHeight(), UnitPixel);
	graphics.DrawImage(img[3], Rect(20, 630, img[3]->GetWidth(), img[3]->GetHeight()), 0, 0, img[3]->GetWidth(), img[3]->GetHeight(), UnitPixel);

	// <<

}

void GameManager::DrawClearScreen(Graphics & graphics)
{
}

void GameManager::DrawOverScreen(Graphics & graphics)
{
}

void GameManager::DrawEndScreen(Graphics & graphics)
{
	sound.ReleaseSound();
}


