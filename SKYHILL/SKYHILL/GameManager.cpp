#include "GameManager.h"

void GameManager::Init()
{
	string BG_path = "Sound/rain.wav";
	string EFF_path = "Sound/click.wav";

	sound.CreateBGSound(1, &BG_path);
	sound.CreateEFFSound(1, &EFF_path);

	for (int i = 0; i < 4; i++) {
		passive[i] = false;
		active[i] = false;
	}


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
			else if(chk_op && !chk_game && !chk_ability)
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
				}
			}
		}
		break;
	case INGAME:
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



void GameManager::DrawStartScreen(Graphics & graphics)
{

	static int count = 0;

	Image* pImg = Image::FromFile((WCHAR*)L"images/Lobby.png");
	Image* set_icon = Image::FromFile((WCHAR*)L"images/Setting.png");
	Image* end_icon = Image::FromFile((WCHAR*)L"images/End.png");
	Image* play_icon = Image::FromFile((WCHAR*)L"images/Play.png");

	// ���� �κ� ȭ���� ������ ����Ǹ� ���� �κк��� �ݺ����
	if (count >= 22600) count = 4520;

	if (!pImg) return;

	// ��� ����
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(240, 0, 240), Color(255, 10, 255));

	int w = set_icon->GetWidth();
	int h = set_icon->GetHeight();
	graphics.DrawImage(pImg, Rect(0, 0, screenRect.right, screenRect.bottom), count, 0, 1120, 762, UnitPixel);
	graphics.DrawImage(set_icon, Rect(screenRect.right - 225, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(play_icon, Rect(screenRect.right - 150, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(end_icon, Rect(screenRect.right - 75, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);

	// �ɼ� �������� ���� ���
	if (chk_op) {
		// >> : ��� ��Ӱ�
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// �ɼ� â ���
		Image* op_icon = Image::FromFile((WCHAR*)L"images/Option.png");
		int o_w = op_icon->GetWidth();
		int o_h = op_icon->GetHeight();
		graphics.DrawImage(op_icon, Rect(screenRect.right / 2 - 188, screenRect.bottom / 2 - 116, o_w, o_h), 0, 0, o_w, o_h, UnitPixel);

		// �ؽ�Ʈ �Է�
		brush.SetColor(Color(255, 145, 141, 120));
		
		// ��Ʈ ����
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, 15, FontStyleBold, UnitPixel);

		// �����, ȿ���� �� ���
		TCHAR BG[20], EFF[20];
		wsprintf(BG, TEXT("����� : %d%%"), (int)(sound.getM_vol()*100));
		wsprintf(EFF, TEXT("ȿ���� : %d%%"), (int)(sound.getE_vol()*100));
		PointF pointF1(screenRect.right / 2 - 50.0f, screenRect.bottom / 2 - 45.0f);
		graphics.DrawString(BG, -1, &font, pointF1, &brush);
		PointF pointF2(screenRect.right / 2 - 50.0f, screenRect.bottom / 2 + 10.0f);
		graphics.DrawString(EFF, -1, &font, pointF2, &brush);

		delete op_icon;
	}
	if (chk_game && !chk_ability)
	{
		// >> : ��� ��Ӱ�
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// ���̵� ���� â ���
		Image* diff_icon = Image::FromFile((WCHAR*)L"images/Difficult.png"); // �ɼ� â
		int d_w = diff_icon->GetWidth();
		int d_h = diff_icon->GetHeight();
		graphics.DrawImage(diff_icon, Rect(screenRect.right / 2 - 112, screenRect.bottom / 2 - 119, d_w, d_h), 0, 0, d_w, d_h, UnitPixel);

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

		delete diff_icon;
	}
	if (chk_ability)
	{
		// >> : ��� ��Ӱ�
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// ���̵� ���� â ���
		Image* ability_icon = Image::FromFile((WCHAR*)L"images/Ability2.png"); // �ɼ� â
		int a_w = ability_icon->GetWidth();
		int a_h = ability_icon->GetHeight();
		graphics.DrawImage(ability_icon, Rect(screenRect.right / 2 - 423, screenRect.bottom / 2 - 199, a_w, a_h), 0, 0, a_w, a_h, UnitPixel);

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
		delete ability_icon;
	}
	count += 1130;
	delete pImg;
	delete set_icon;
	delete end_icon;
	delete play_icon;

}

void GameManager::DrawGameScreen(Graphics & graphics)
{
	SolidBrush brush(Color(255, 0, 0, 0));
	brush.SetColor(Color(128, 0, 0, 0));
	graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
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

