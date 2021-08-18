#include "GameManager.h"

// 꼭대기 층 인경우 
// 꼭대기 층 크기 : 왼쪽 배경 140, 205 + 1층 1010, 205 + 오른쪽 배경 130, 205
// 방 1칸당 크기 : 336, 210, 계단방 크기 : 305, 210, 층 사이 벽 높이 = 30
// 1층당 크기 : 왼쪽 배경 137, 210 + 왼쪽방 336, 210 + 벽 18, 210 + 계단방 크기 307, 202 + 벽 9, 202 + 오른쪽방 337, 202 + 오른쪽 배경 130, 202
Rect rooms[] = { Rect(135, 466, 355, 215), Rect(490, 466, 315, 215), Rect(805, 466, 355, 215) // 아래층 왼쪽, 계단방, 아래층 오른쪽
			   , Rect(135, 228, 355, 215), Rect(490, 228, 315, 215), Rect(805, 228, 355, 215) // 중간층 왼쪽, 계단방, 중간층 오른쪽
			   , Rect(135, 0, 355, 215), Rect(490, 0, 315, 215), Rect(805, 0, 355, 215) }; // 윗층 왼쪽 , 계단방, 윗층 오른쪽

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
		else if (chk_op && !chk_game && !chk_ability)
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
		else if (!chk_op && chk_game && !chk_ability) // 게임 난이도 선택단계
		{
			//screenRect.right / 2 - 112, screenRect.bottom / 2 - 119

			// 쉬움 버튼 클릭
			if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
				&& y >= screenRect.bottom / 2 - 75 && y < screenRect.bottom / 2 - 51)
			{
				// 난이도에 따른 설정
				chk_ability = true;
				difficulty = 1;
				// 
			}
			// 보통 버튼 클릭
			else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
				&& y >= screenRect.bottom / 2 - 32 && y < screenRect.bottom / 2 - 8)
			{
				// 난이도에 따른 설정
				chk_ability = true;
				difficulty = 2;
			}
			// 어려움 버튼 클릭
			else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
				&& y >= screenRect.bottom / 2 + 17 && y < screenRect.bottom / 2 + 41)
			{
				// 난이도에 따른 설정
				chk_ability = true;
				difficulty = 3;
			}
			// 취소 버튼 클릭
			else if (x >= screenRect.right / 2 - 100 && x < screenRect.right / 2 + 98
				&& y >= screenRect.bottom / 2 + 82 && y < screenRect.bottom / 2 + 106)
			{
				chk_game = false;
			}
		}
		else if (chk_ability)
		{
			// 플레이어가 직접 사용
			if (x >= screenRect.right / 2 - 133 && x < screenRect.right / 2 - 71
				&& y >= screenRect.bottom / 2 - 85 && y < screenRect.bottom / 2 - 23)
			{// 능력 없음
				for (int i = 0; i < 4; i++)
				{
					if (i == 0) passive[i] = true;
					else passive[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 67 && x < screenRect.right / 2 - 5
				&& y >= screenRect.bottom / 2 - 85 && y < screenRect.bottom / 2 - 23)
			{// 응급 도구
				for (int i = 0; i < 4; i++)
				{
					if (i == 1) passive[i] = true;
					else passive[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 1 && x < screenRect.right / 2 + 61
				&& y >= screenRect.bottom / 2 - 85 && y < screenRect.bottom / 2 - 23)
			{// 생존 키트
				for (int i = 0; i < 4; i++)
				{
					if (i == 2) passive[i] = true;
					else passive[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 + 65 && x < screenRect.right / 2 + 127
				&& y >= screenRect.bottom / 2 - 85 && y < screenRect.bottom / 2 - 23)
			{// 운좋은 놈
				for (int i = 0; i < 4; i++)
				{
					if (i == 3) passive[i] = true;
					else passive[i] = false;
				}
			}
			// 자동으로 적용
			else if (x >= screenRect.right / 2 - 133 && x < screenRect.right / 2 - 71
				&& y >= screenRect.bottom / 2 + 29 && y < screenRect.bottom / 2 + 91)
			{// 없음
				for (int i = 0; i < 4; i++)
				{
					if (i == 0) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 67 && x < screenRect.right / 2 - 5
				&& y >= screenRect.bottom / 2 + 29 && y < screenRect.bottom / 2 + 91)

			{// 운이 좌우하는 게임
				for (int i = 0; i < 4; i++)
				{
					if (i == 1) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 1 && x < screenRect.right / 2 + 61
				&& y >= screenRect.bottom / 2 + 29 && y < screenRect.bottom / 2 + 91)
			{// 엉망진창
				for (int i = 0; i < 4; i++)
				{
					if (i == 2) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 + 65 && x < screenRect.right / 2 + 127
				&& y >= screenRect.bottom / 2 + 29 && y < screenRect.bottom / 2 + 91)
			{// 고소공포증
				for (int i = 0; i < 4; i++)
				{
					if (i == 3) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 257 && x < screenRect.right / 2 - 56
				&& y >= screenRect.bottom / 2 + 153 && y < screenRect.bottom / 2 + 177)
			{ // 닫기
				chk_ability = false;
			}
			else if (x >= screenRect.right / 2 + 57 && x < screenRect.right / 2 + 255
				&& y >= screenRect.bottom / 2 + 153 && y < screenRect.bottom / 2 + 177)
			{ // 게임시작
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
			// rooms[0] ~ [2] = 맨 아래층 -> 꼭대기 층이므로 아래로만 이동 가능
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
			// rooms[0] ~ [2] = 맨 아래층 rooms[3] ~ [5] = 중간 층 , rooms[6]~ [8] = 맨 윗층
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

	// 메인 로비 화면이 끝까지 재생되면 지정 부분부터 반복재생
	if (count >= 22600) count = 4520;

	//

	// 배경 제거
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(240, 0, 240), Color(255, 10, 255));

	int w = img[1]->GetWidth();
	int h = img[1]->GetHeight();
	graphics.DrawImage(img[0], Rect(0, 0, screenRect.right, screenRect.bottom), count, 0, 1120, 762, UnitPixel);
	graphics.DrawImage(img[1], Rect(screenRect.right - 225, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(img[2], Rect(screenRect.right - 150, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);
	graphics.DrawImage(img[3], Rect(screenRect.right - 75, screenRect.bottom - 100, w, h), 0, 0, w, h, UnitPixel, &imgAttr);

	// 옵션 아이콘을 누른 경우
	if (chk_op) {
		// >> : 배경 어둡게
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// 옵션 창 출력
		int o_w = img[4]->GetWidth();
		int o_h = img[4]->GetHeight();
		graphics.DrawImage(img[4], Rect(screenRect.right / 2 - 188, screenRect.bottom / 2 - 116, o_w, o_h), 0, 0, o_w, o_h, UnitPixel);

		// 텍스트 입력
		brush.SetColor(Color(255, 145, 141, 120));

		// 폰트 설정
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, 15, FontStyleBold, UnitPixel);

		// 배경음, 효과음 값 출력
		TCHAR BG[20], EFF[20];
		wsprintf(BG, TEXT("배경음 : %d%%"), (int)(sound.getM_vol() * 100));
		wsprintf(EFF, TEXT("효과음 : %d%%"), (int)(sound.getE_vol() * 100));
		PointF pointF1(screenRect.right / 2 - 50.0f, screenRect.bottom / 2 - 45.0f);
		graphics.DrawString(BG, -1, &font, pointF1, &brush);
		PointF pointF2(screenRect.right / 2 - 50.0f, screenRect.bottom / 2 + 10.0f);
		graphics.DrawString(EFF, -1, &font, pointF2, &brush);
	}
	if (chk_game && !chk_ability)
	{
		// >> : 배경 어둡게
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// 난이도 선택 창 출력
		int d_w = img[5]->GetWidth();
		int d_h = img[5]->GetHeight();
		graphics.DrawImage(img[5], Rect(screenRect.right / 2 - 112, screenRect.bottom / 2 - 119, d_w, d_h), 0, 0, d_w, d_h, UnitPixel);

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
	if (chk_ability)
	{
		// >> : 배경 어둡게
		SolidBrush brush(Color(255, 0, 0, 0));
		brush.SetColor(Color(128, 0, 0, 0));
		graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
		// <<

		// 능력 선택 창 출력
		int a_w = img[6]->GetWidth();
		int a_h = img[6]->GetHeight();
		graphics.DrawImage(img[6], Rect(screenRect.right / 2 - 423, screenRect.bottom / 2 - 199, a_w, a_h), 0, 0, a_w, a_h, UnitPixel);

		// 선택한 능력 표시
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
					wsprintf(temp, TEXT("응급 처치 : 1회 사용 가능한 버튼이 생긴다. 사용 시, HP를 50만큼 회복시켜준다."));
				}
				else if (i == 2)
				{
					wsprintf(temp, TEXT("생존 키트 : 1회 사용 가능한 버튼이 생긴다. 사용 시 배고픔을 100만큼 회복시켜준다."));
				}
				else if (i == 3)
				{
					wsprintf(temp, TEXT("운 좋은 놈 : 150회 이동 후 재사용 가능. 사용 시 1000의 대미지 상대를 즉사시킨다."));
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
					wsprintf(temp, TEXT("운이 좌우 하는 게임 : 적을 타격할 시 기본 공격 대미지가 1 ~ 300%%로 랜덤한 특성"));
				}
				else if (i == 2)
				{
					wsprintf(temp, TEXT("엉망진창 : 아이템 드롭 확률이 50%% 증가, 10회 이동마다 임의의 아이템을 잃어버리는 특성"));
				}
				else if (i == 3)
				{
					wsprintf(temp, TEXT("고소공포증 : 시작 최대 HP가 60이나, 5개 층을 내려갈때마다 최대 HP가 10씩 증가한다."));
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
		1. 플레이어가 상 - 하로 움직일 때는 카메라 시점 변경 o
		2. 플레이어가 좌 - 우로 움직일 때는 카메라 시점 변경 x
		3. 플레이어가 대각방향으로 클릭했을 경우, 상, 하가 먼저 실행 된 후 좌 우 실행
		4. 맵 -> 캐릭터 -> 벽 순서로 그려주고, 방문하지 않은 방은 검은색 사각형으로 가려져있음
		(플레이어가 이동할 때 계단을 내려가거나 문을 지나가므로 벽(문)의 위치에 맞게 검은사각형 그려주기)
		5. 현재 위치한 방은 검은색 사각형이 투명, 방문했던 방은 검은색 사각형이 반투명

		=============||=============||=============
					 ||				||
					 ||				||
		  ↖ = ↑ ←	 ||		 ↑		||   ↗ = ↑ →
					 ||				||
					 ||				||
		=============||=============||=============
		=============||=============||=============
					 ||				||
					 ||				||
			←		 ||				||		→
					 ||				||
					 ||				||
		=============||=============||=============
		=============||=============||=============
					 ||				||
					 ||				||
		  ↙ = ↓ ←	 ||		 ↓		||	↘ = ↓ →
					 ||				||
					 ||				||
		=============||=============||=============

	*/



	SolidBrush brush(Color(255, 0, 0, 0));


	static int m_h = 20;
	static int b_h = 0;

	Rect move_rooms[] = { Rect(135, 690 - b_h, 355, 230), Rect(490, 700 - b_h, 320, 215), Rect(805, 700 - b_h, 360, 230) // 아래층 왼쪽, 계단방, 아래층 오른쪽
			 , Rect(135, 463 - b_h, 355, 230), Rect(490, 473 - b_h, 320, 215), Rect(805, 463 - b_h, 360, 230) // 중간층 왼쪽, 계단방, 중간층 오른쪽
			 , Rect(135, 225 - b_h, 355, 230), Rect(490, 235 - b_h, 320, 215), Rect(805, 225 - b_h, 360, 230)
			 , Rect(135, 0 - b_h, 355, 230), Rect(490, 0 - b_h, 320, 215), Rect(805, 0 - b_h, 360, 230) }; // 윗층 왼쪽 , 계단방, 윗층 오른쪽
	if (move_chk) // 플레이어가 움직임
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
				// 정상 이미지 출력 : graphics.DrawImage(img, 0, 0, 100, 100);
				// y축 대칭 출력 : graphics.DrawImage(img, 100, 0, -100, 100);
				// 방들

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
							if (room_chk[i][j] == 1) // 한번 방문 했던 곳이라면
							{
								brush.SetColor(Color(128, 0, 0, 0)); // 반투명한 검은 사각형
								graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
							}
							else if (room_chk[i][j] == 0)
							{
								brush.SetColor(Color(255, 0, 0, 0)); // 검은 사각형
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
							if (room_chk[i][j] == 1) // 한번 방문 했던 곳이라면
							{
								brush.SetColor(Color(128, 0, 0, 0)); // 반투명한 검은 사각형
								graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
							}
							else if (room_chk[i][j] == 0)
							{
								brush.SetColor(Color(255, 0, 0, 0)); // 검은 사각형
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
				// 계단 내려갈때
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
	else  // 플레이어가 멈춤
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
			// 검은색 사각형으로 방문한 곳 방문하지 않은곳 그리기.
			int cnt = 0;
			for (int i = floor - 1; i <= floor + 1 && i < 30; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (room_chk[i][j] == 1) // 한번 방문 했던 곳이라면
					{
						brush.SetColor(Color(128, 0, 0, 0)); // 반투명한 검은 사각형
						graphics.FillRectangle(&brush, rooms[cnt * 3 + j]);
					}
					else if (room_chk[i][j] == 0)
					{
						brush.SetColor(Color(255, 0, 0, 0)); // 검은 사각형
						graphics.FillRectangle(&brush, rooms[cnt * 3 + j]);
					}
				}
				cnt++;
			}
		}
		// 29층 부터
		else if (floor < 30 && floor > 1)
		{
			graphics.DrawImage(img[0], Rect(0, 0, screenRect.right, screenRect.bottom), 0, m_h, 1280, 700, UnitPixel);
			graphics.DrawImage(img[2], Rect(screenRect.right / 2 - 135, screenRect.bottom / 2 - 20, 90, 120), p_w, p_h, 170, 318, UnitPixel);
			int cnt = 0;
			for (int i = floor - 1; i <= floor + 1 && i <= 30; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (room_chk[i][j] == 1) // 한번 방문 했던 곳이라면
					{
						brush.SetColor(Color(128, 0, 0, 0)); // 반투명한 검은 사각형
						graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
					}
					else if (room_chk[i][j] == 0)
					{
						brush.SetColor(Color(255, 0, 0, 0)); // 검은 사각형
						graphics.FillRectangle(&brush, move_rooms[cnt * 3 + j]);
					}
				}
				cnt++;
			}
		}
		// 1층인 경우
		else
		{

		}

		p_w += 170;


	}




	// UI 그리기
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


