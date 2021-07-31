#include "GameManager.h"

void GameManager::Init()
{
	sound->Init();
	sound = new CSound("Sound/rain.wav", true);
	sound->Add_effect("Sound/click.wav", 1);
	sound->play(0);
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
	sound->Update();

	if (nFlags == MK_LBUTTON) {
		sound->play(1);
		
	}
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
	sound->stop();
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
	DrawStartScreen(graphics);

	//switch (state)
	//{
	//case START:
	//	DrawStartScreen(graphics);
	//	break;
	//case INGAME:
	//	DrawGameScreen(graphics);
	//	break;
	//case END:
	//	DrawEndScreen(graphics);
	//	break;
	//case CLEAR:
	//	DrawClearScreen(graphics);
	//	break;
	//case OVER:
	//	DrawOverScreen(graphics);
	//	break;
	//}
}

void GameManager::GDI_End()
{
	GdiplusShutdown(g_GdiToken);
}

void GameManager::DrawStartScreen(Graphics & graphics)
{

	static int count = 0;

	Image* pImg = Image::FromFile((WCHAR*)L"images/Lobby.png");
	Image* set_icon = Image::FromFile((WCHAR*)L"images/Setting.png");
	Image* end_icon = Image::FromFile((WCHAR*)L"images/End.png");
	Image* play_icon = Image::FromFile((WCHAR*)L"images/Play.png");
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(71, 71, 71), Color(92, 92, 92));

	if (count > 22600) count = 4520;

	if (!pImg)
		return;

	int w = set_icon->GetWidth();
	int h = set_icon->GetHeight();
	graphics.DrawImage(pImg, Rect(0, 0, screenRect.right, screenRect.bottom), count, 0, 1120, 762, UnitPixel);
	graphics.DrawImage(set_icon, Rect(screenRect.right - 225, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(play_icon, Rect(screenRect.right - 150, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(end_icon, Rect(screenRect.right - 75, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);


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
	delete sound;
	CSound::Release();
}
