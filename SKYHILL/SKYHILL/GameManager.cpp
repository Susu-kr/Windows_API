#include "GameManager.h"

void GameManager::Init()
{
	string BG_path = "Sound/rain.wav";
	string EFF_path = "Sound/click.wav";

	sound.CreateBGSound(1, &BG_path);
	sound.CreateEFFSound(1, &EFF_path);

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
			if (!chk_op && !chk_game)
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
			else if(chk_op && !chk_game)
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
			else if (!chk_op && chk_game) // ���� ���̵� ���ôܰ�
			{
				//screenRect.right / 2 - 112, screenRect.bottom / 2 - 119

				// ���� ��ư Ŭ��
				if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
					&& y >= screenRect.bottom / 2 - 75 && y < screenRect.bottom / 2 - 51)
				{
					// ���̵��� ���� ����

					// 
				}
				// ���� ��ư Ŭ��
				else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
					&& y >= screenRect.bottom / 2 - 32 && y < screenRect.bottom / 2 - 8)
				{
					// ���̵��� ���� ����

				}
				// ����� ��ư Ŭ��
				else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
					&& y >= screenRect.bottom / 2 + 17 && y < screenRect.bottom / 2 + 41)
				{
					// ���̵��� ���� ����

				}
				// ��� ��ư Ŭ��
				else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
					&& y >= screenRect.bottom / 2 + 82 && y < screenRect.bottom / 2 + 106)
				{
					chk_game = false;
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
	if (chk_game)
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
	}
	count += 1130;
	delete pImg;
	delete set_icon;
	delete end_icon;
	delete play_icon;

}

void GameManager::DrawGameScreen(Graphics & graphics)
{
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

