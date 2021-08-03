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
				// 옵션버튼 클릭
				if (x >= screenRect.right - 225 && x < screenRect.right - 175
					&& y >= screenRect.bottom - 100 && y < screenRect.bottom - 50)
				{
					chk_op = true;
				}
				// 게임시작버튼 클릭
				else if (x >= screenRect.right - 150 && x < screenRect.right - 100
					&& y >= screenRect.bottom - 100 && y < screenRect.bottom - 50)
				{
					chk_game = true;
				}
				// 나가기버튼 클릭
				else if (x >= screenRect.right - 75 && x < screenRect.right - 25
					&& y >= screenRect.bottom - 100 && y < screenRect.bottom - 50)
				{
					return false;
				}
			}
			else if(chk_op && !chk_game)
			{
				// 배경음버튼 클릭(감소)
				if (x >= screenRect.right / 2 - 180 && x < screenRect.right / 2 - 155
					&& y >= screenRect.bottom / 2 - 50 && y < screenRect.bottom / 2 - 25)
				{
					sound.volumeDown();
				}

				// 배경음버튼 클릭(증가)
				else if (x >= screenRect.right / 2 + 150 && x < screenRect.right / 2 + 175
					&& y >= screenRect.bottom / 2 - 50 && y < screenRect.bottom / 2 - 25)
				{
					sound.volumeUp();
				}

				// 효과음버튼 클릭(감소)
				else if (x >= screenRect.right / 2 - 180 && x < screenRect.right / 2 - 155
					&& y >= screenRect.bottom / 2 && y < screenRect.bottom / 2 + 25)
				{
					sound.e_volumeDown();
				}
				// 효과음버튼 클릭(증가)
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
			else if (!chk_op && chk_game) // 게임 난이도 선택단계
			{
				//screenRect.right / 2 - 112, screenRect.bottom / 2 - 119

				// 쉬움 버튼 클릭
				if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
					&& y >= screenRect.bottom / 2 - 75 && y < screenRect.bottom / 2 - 51)
				{
					// 난이도에 따른 설정

					// 
				}
				// 보통 버튼 클릭
				else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
					&& y >= screenRect.bottom / 2 - 32 && y < screenRect.bottom / 2 - 8)
				{
					// 난이도에 따른 설정

				}
				// 어려움 버튼 클릭
				else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
					&& y >= screenRect.bottom / 2 + 17 && y < screenRect.bottom / 2 + 41)
				{
					// 난이도에 따른 설정

				}
				// 취소 버튼 클릭
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

	// 메인 로비 화면이 끝까지 재생되면 지정 부분부터 반복재생
	if (count >= 22600) count = 4520;

	if (!pImg) return;

	// 배경 제거
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(240, 0, 240), Color(255, 10, 255));

	int w = set_icon->GetWidth();
	int h = set_icon->GetHeight();
	graphics.DrawImage(pImg, Rect(0, 0, screenRect.right, screenRect.bottom), count, 0, 1120, 762, UnitPixel);
	graphics.DrawImage(set_icon, Rect(screenRect.right - 225, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(play_icon, Rect(screenRect.right - 150, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(end_icon, Rect(screenRect.right - 75, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);

	// 옵션 아이콘을 누른 경우
	if (chk_op) {
		// >> : 배경 어둡게
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// 옵션 창 출력
		Image* op_icon = Image::FromFile((WCHAR*)L"images/Option.png");
		int o_w = op_icon->GetWidth();
		int o_h = op_icon->GetHeight();
		graphics.DrawImage(op_icon, Rect(screenRect.right / 2 - 188, screenRect.bottom / 2 - 116, o_w, o_h), 0, 0, o_w, o_h, UnitPixel);

		// 텍스트 입력
		brush.SetColor(Color(255, 145, 141, 120));
		
		// 폰트 설정
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, 15, FontStyleBold, UnitPixel);

		// 배경음, 효과음 값 출력
		TCHAR BG[20], EFF[20];
		wsprintf(BG, TEXT("배경음 : %d%%"), (int)(sound.getM_vol()*100));
		wsprintf(EFF, TEXT("효과음 : %d%%"), (int)(sound.getE_vol()*100));
		PointF pointF1(screenRect.right / 2 - 50.0f, screenRect.bottom / 2 - 45.0f);
		graphics.DrawString(BG, -1, &font, pointF1, &brush);
		PointF pointF2(screenRect.right / 2 - 50.0f, screenRect.bottom / 2 + 10.0f);
		graphics.DrawString(EFF, -1, &font, pointF2, &brush);

		delete op_icon;
	}
	if (chk_game)
	{
		// >> : 배경 어둡게
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// 난이도 선택 창 출력
		Image* diff_icon = Image::FromFile((WCHAR*)L"images/Difficult.png"); // 옵션 창
		int d_w = diff_icon->GetWidth();
		int d_h = diff_icon->GetHeight();
		graphics.DrawImage(diff_icon, Rect(screenRect.right / 2 - 112, screenRect.bottom / 2 - 119, d_w, d_h), 0, 0, d_w, d_h, UnitPixel);

		// 폰트 설정
		brush.SetColor(Color(255, 145, 141, 120));
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, 15, FontStyleBold, UnitPixel);

		// 난이도 출력
		PointF pointF1(screenRect.right / 2 - 22.0f, screenRect.bottom / 2 - 72.0f);
		graphics.DrawString(L"쉬 움", -1, &font, pointF1, &brush);
		PointF pointF2(screenRect.right / 2 - 22.0f, screenRect.bottom / 2 - 30.0f);
		graphics.DrawString(L"보 통", -1, &font, pointF2, &brush);
		PointF pointF3(screenRect.right / 2 - 32.0f, screenRect.bottom / 2 + 22.0f);
		graphics.DrawString(L"어 려 움", -1, &font, pointF3, &brush);
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

