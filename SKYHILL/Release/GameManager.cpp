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
	string BG_path[2] = { "Sound/Main.wav", "Sound/InGame.wav" };
	string EFF_path[6] = { "Sound/click.wav", "Sound/foot.wav", "Sound/Hit.wav", "Sound/Hit2.wav", "Sound/Enemy_Idle.wav", "Sound/Enemy_Death.wav" };
	sound.CreateBGSound(2, BG_path);
	sound.CreateEFFSound(6, EFF_path);
	loadImage(state);
	passive[0] = true;
	active[0] = true;
	srand(unsigned int(time(NULL)));
	for (int i = 2; i < 29; i++) 
	{
		int num = rand() % 4;
		if (num >= 0 && num <= 2) {
			Enemy_room[i][num] = true;
			Enemy_HP[i][num] = 15.7;
			Enemy_Death[i][num] = false;
		}
	}
	Player_Attack_Percent[0] = 40 + Player_Stat[3] * 2;
	Player_Attack_Percent[1] = 80 + Player_Stat[3] * 2;
	Player_Attack_Percent[2] = 70 + Player_Stat[3] * 2;
	Player_Damage[0] = 1.0;
	Player_Damage[1] = 2.0;
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

	if (Player_Exp >= Player_NextExp)
	{
		Player_Exp -= Player_NextExp;
		Player_NextExp = Player_NextExp * 1.5;
		Player_Level++;
		Player_Stat[0] += 2;
		Player_Stat[3] += 2;

		Player_HP += 20.0;
		Player_Hunger += 15;
	}

	Player_Attack_Percent[0] = 40 + Player_Stat[3] * 2;
	Player_Attack_Percent[1] = 80 + Player_Stat[3] * 2;
	Player_Attack_Percent[2] = 70 + Player_Stat[3] * 2;

	for (int i = 0; i < 3; i++)
	{
		if (Player_Attack_Percent[i] >= 95) Player_Attack_Percent[i] = 95;
	}

	Player_Damage[0] = 0.2 * Player_Stat[0];
	Player_Damage[1] = 0.4 * Player_Stat[0];

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
	sound.PlaySoundBG(1);
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
	for (int i = 0; i < 20; i++)
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
			if (x >= screenRect.right / 2 - 170 && x < screenRect.right / 2 - 145
				&& y >= screenRect.bottom / 2 - 50 && y < screenRect.bottom / 2 - 25)
			{
				sound.volumeDown();
			}

			// �������ư Ŭ��(����)
			else if (x >= screenRect.right / 2 + 170 && x < screenRect.right / 2 + 185
				&& y >= screenRect.bottom / 2 - 50 && y < screenRect.bottom / 2 - 25)
			{
				sound.volumeUp();
			}

			// ȿ������ư Ŭ��(����)
			else if (x >= screenRect.right / 2 - 170 && x < screenRect.right / 2 - 145
				&& y >= screenRect.bottom / 2 + 10 && y < screenRect.bottom / 2 + 35)
			{
				sound.e_volumeDown();
			}
			// ȿ������ư Ŭ��(����)
			else if (x >= screenRect.right / 2 + 170 && x < screenRect.right / 2 + 185
				&& y >= screenRect.bottom / 2 + 10 && y < screenRect.bottom / 2 + 35)
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
			if (x >= screenRect.right / 2 - 90 && x < screenRect.right / 2 + 108
				&& y >= screenRect.bottom / 2 - 65 && y < screenRect.bottom / 2 - 41)
			{
				// ���̵��� ���� ����
				chk_ability = true;
				difficulty = 1;
				Player_HP = 100.0;
				Player_Hunger = 80;
				Enemy_Damage[0] = 1.5;
				Enemy_Damage[1] = 2.0;
				Enemy_EXP = 27;
				// 
			}
			// ���� ��ư Ŭ��
			else if (x >= screenRect.right / 2 - 90 && x < screenRect.right / 2 + 108
				&& y >= screenRect.bottom / 2 - 22 && y < screenRect.bottom / 2 + 2)
			{
				// ���̵��� ���� ����
				chk_ability = true;
				difficulty = 2;
				Player_HP = 80.0;
				Player_Hunger = 60;
				Enemy_Damage[0] = 2.0;
				Enemy_Damage[1] = 2.5;
				Enemy_EXP = 21;
			}
			// ����� ��ư Ŭ��
			else if (x >= screenRect.right / 2 - 90 && x < screenRect.right / 2 + 108
				&& y >= screenRect.bottom / 2 + 27 && y < screenRect.bottom / 2 + 51)
			{
				// ���̵��� ���� ����
				chk_ability = true;
				difficulty = 3;
				Player_HP = 60.0;
				Player_Hunger = 40;
				Enemy_Damage[0] = 2.5;
				Enemy_Damage[1] = 3.0;
				Enemy_EXP = 18;
			}
			// ��� ��ư Ŭ��
			else if (x >= screenRect.right / 2 - 90 && x < screenRect.right / 2 + 108
				&& y >= screenRect.bottom / 2 + 92 && y < screenRect.bottom / 2 + 116)
			{
				chk_game = false;
			}
		}
		else if (chk_ability)
		{
			// �÷��̾ ���� ���
			if (x >= screenRect.right / 2 - 113 && x < screenRect.right / 2 - 51
				&& y >= screenRect.bottom / 2 - 75 && y < screenRect.bottom / 2 - 13)
			{// �ɷ� ����
				for (int i = 0; i < 4; i++)
				{
					if (i == 0) passive[i] = true;
					else passive[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 57 && x < screenRect.right / 2 + 5
				&& y >= screenRect.bottom / 2 - 75 && y < screenRect.bottom / 2 - 13)
			{// ���� ����
				for (int i = 0; i < 4; i++)
				{
					if (i == 1) passive[i] = true;
					else passive[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 + 20 && x < screenRect.right / 2 + 81
				&& y >= screenRect.bottom / 2 - 75 && y < screenRect.bottom / 2 - 13)
			{// ���� ŰƮ
				for (int i = 0; i < 4; i++)
				{
					if (i == 2) passive[i] = true;
					else passive[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 + 85 && x < screenRect.right / 2 + 147
				&& y >= screenRect.bottom / 2 - 75 && y < screenRect.bottom / 2 - 13)
			{// ������ ��
				for (int i = 0; i < 4; i++)
				{
					if (i == 3) passive[i] = true;
					else passive[i] = false;
				}
			}
			// �ڵ����� ����
			else if (x >= screenRect.right / 2 - 113 && x < screenRect.right / 2 - 51
				&& y >= screenRect.bottom / 2 + 39 && y < screenRect.bottom / 2 + 101)
			{// ����
				for (int i = 0; i < 4; i++)
				{
					if (i == 0) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 57 && x < screenRect.right / 2 + 5
				&& y >= screenRect.bottom / 2 + 39 && y < screenRect.bottom / 2 + 101)

			{// ���� �¿��ϴ� ����
				for (int i = 0; i < 4; i++)
				{
					if (i == 1) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 + 20 && x < screenRect.right / 2 + 81
				&& y >= screenRect.bottom / 2 + 39 && y < screenRect.bottom / 2 + 101)
			{// ������â
				for (int i = 0; i < 4; i++)
				{
					if (i == 2) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 + 85 && x < screenRect.right / 2 + 147
				&& y >= screenRect.bottom / 2 + 39 && y < screenRect.bottom / 2 + 101)
			{// ��Ұ�����
				for (int i = 0; i < 4; i++)
				{
					if (i == 3) active[i] = true;
					else active[i] = false;
				}
			}
			else if (x >= screenRect.right / 2 - 237 && x < screenRect.right / 2 - 36
				&& y >= screenRect.bottom / 2 + 163 && y < screenRect.bottom / 2 + 187)
			{ // �ݱ�
				chk_ability = false;
			}
			else if (x >= screenRect.right / 2 + 77 && x < screenRect.right / 2 + 275
				&& y >= screenRect.bottom / 2 + 163 && y < screenRect.bottom / 2 + 187)
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
			//if (x >= rooms[0].GetLeft() && x < rooms[0].GetRight() && y >= rooms[0].GetTop() && y < rooms[0].GetBottom())
			//{
			//	int cnt = 0;
			//	while (cnt < 3)
			//	{
			//		room_chk[floor][cnt] = 1;
			//		cnt++;
			//	}
			//	room_chk[floor-1][1] = 2;
			//	move_chk = true;
			//	direction.push_back(2);
			//	direction.push_back(3);
			//}
			if (x >= rooms[1].GetLeft() && x < rooms[1].GetRight() && y >= rooms[1].GetTop() && y < rooms[1].GetBottom())
			{
				sound.PlaySoundEffect(1);
				int cnt = 0;
				while (cnt < 3)
				{
					room_chk[floor][cnt] = 1;
					cnt++;
				}
				room_chk[floor-1][1] = 2;
				move_chk = true;
				direction.push_back(2);
				player_room = 1;
				if (Player_Hunger == 0) Player_HP -= 2.0;
				else Player_Hunger -= rand() % 4 - 2;

			}
			//else if (x >= rooms[2].GetLeft() && x < rooms[2].GetRight() && y >= rooms[2].GetTop() && y < rooms[2].GetBottom())
			//{
			//	int cnt = 0;
			//	while (cnt < 3)
			//	{
			//		room_chk[floor][cnt] = 1;
			//		cnt++;
			//	}
			//	room_chk[floor-1][1] = 2;
			//	move_chk = true;
			//	direction.push_back(2);
			//	direction.push_back(4);
			//}
		}
		else if (floor < 30 && floor > 1)
		{
			if (!Enemy_room[floor][player_room])
			{
				// rooms[0] ~ [2] = �� �Ʒ��� rooms[3] ~ [5] = �߰� �� , rooms[6]~ [8] = �� ����
			//if (x >= rooms[0].GetLeft() && x < rooms[0].GetRight() && y >= rooms[0].GetTop() && y < rooms[0].GetBottom())
			//{
			//	if (room_chk[floor][0] == 2)
			//	{
			//		room_chk[floor][1] = 2;
			//		room_chk[floor][0] = 1;
			//		move_chk = true;
			//		direction.push_back(4);
			//		direction.push_back(2);
			//		direction.push_back(3);
			//	}
			//	else if (room_chk[floor][1] == 2)
			//	{
			//		int cnt = 0;
			//		while (cnt < 3)
			//		{
			//			room_chk[floor][cnt] = 1;
			//			cnt++;
			//		}
			//		room_chk[floor - 1][1] = 2;
			//		move_chk = true;
			//		direction.push_back(2);
			//		direction.push_back(3);
			//	}
			//	else if (room_chk[floor][2] == 2)
			//	{
			//		room_chk[floor][1] = 2;
			//		room_chk[floor][2] = 1;
			//		move_chk = true;
			//		direction.push_back(3);
			//		direction.push_back(2);
			//		direction.push_back(3);
			//	}
			//}
				if (x >= rooms[1].GetLeft() && x < rooms[1].GetRight() && y >= rooms[1].GetTop() && y < rooms[1].GetBottom())
				{
					if (room_chk[floor][1] == 2)
					{
						sound.PlaySoundEffect(1);
						int cnt = 0;
						while (cnt < 3)
						{
							room_chk[floor][cnt] = 1;
							cnt++;
						}
						room_chk[floor - 1][1] = 2;
						move_chk = true;
						direction.push_back(2);
						player_room = 1;
						if (Player_Hunger == 0) Player_HP -= 2.0;
						else Player_Hunger -= (rand() % 4 - 2);
					}
				}
				//else if (x >= rooms[2].GetLeft() && x < rooms[2].GetRight() && y >= rooms[2].GetTop() && y < rooms[2].GetBottom())
				//{

				//}
				else if (x >= rooms[3].GetLeft() && x < rooms[3].GetRight() && y >= rooms[3].GetTop() && y < rooms[3].GetBottom())
				{
					if (room_chk[floor][1] == 2)
					{
						sound.PlaySoundEffect(1);
						room_chk[floor][1] = 1;
						room_chk[floor][0] = 2;
						move_chk = true;
						direction.push_back(3);
						player_room = 0;
						if (Player_Hunger == 0) Player_HP -= 2.0;
						else Player_Hunger -= (rand() % 4 - 2);
					}
				}
				else if (x >= rooms[4].GetLeft() && x < rooms[4].GetRight() && y >= rooms[4].GetTop() && y < rooms[4].GetBottom())
				{
					if (room_chk[floor][1] != 2)
					{
						sound.PlaySoundEffect(1);

						if (room_chk[floor][0] == 2)
						{
							room_chk[floor][1] = 2;
							room_chk[floor][0] = 1;
							move_chk = true;
							direction.push_back(4);
						}
						else if (room_chk[floor][2] == 2)
						{
							room_chk[floor][1] = 2;
							room_chk[floor][2] = 1;

							move_chk = true;
							direction.push_back(3);
						}
						player_room = 1;
						if (Player_Hunger == 0) Player_HP -= 2.0;
						else Player_Hunger -= (rand() % 4 - 2);
					}
				}
				else if (x >= rooms[5].GetLeft() && x < rooms[5].GetRight() && y >= rooms[5].GetTop() && y < rooms[5].GetBottom())
				{
					if (room_chk[floor][1] == 2)
					{
						sound.PlaySoundEffect(1);

						room_chk[floor][1] = 1;
						room_chk[floor][2] = 2;
						move_chk = true;
						direction.push_back(4);
						player_room = 2;
						if (Player_Hunger == 0) Player_HP -= 2.0;
						else Player_Hunger -= (rand() % 4 - 2);
					}
				}
				//else if (x >= rooms[6].GetLeft() && x < rooms[6].GetRight() && y >= rooms[6].GetTop() && y < rooms[6].GetBottom())
				//{
				//}
				else if (x >= rooms[7].GetLeft() && x < rooms[7].GetRight() && y >= rooms[7].GetTop() && y < rooms[7].GetBottom())
				{
					if (room_chk[floor][1] == 2)
					{
						sound.PlaySoundEffect(1);
						int cnt = 0;
						while (cnt < 3)
						{
							room_chk[floor][cnt] = 1;
							cnt++;
						}
						room_chk[floor + 1][1] = 2;
						move_chk = true;
						direction.push_back(1);
						player_room = 1;
						if (Player_Hunger == 0) Player_HP -= 2.0;
						else Player_Hunger -= (rand() % 4 - 2);
					}
				}
				//else if (x >= rooms[8].GetLeft() && x < rooms[8].GetRight() && y >= rooms[8].GetTop() && y < rooms[8].GetBottom())
				//{
				//}
			}
			
		}

		if (Enemy_room[floor][player_room])
		{
			E_D = (float)((float)((rand() % (int)((Enemy_Damage[1] - Enemy_Damage[0]) * 10)) + (int)(Enemy_Damage[0] * 10)) / 10.0);
			P_D = 0.0;
			int num = rand() & 100 + 1;
			// ���� �ɼ� ����
			if (x >= screenRect.right / 2 - 35 && x < screenRect.right / 2 + 35 && y >= screenRect.bottom - 70 && y < screenRect.bottom - 10)
			{
				if (!attack_op) attack_op = true;
				else attack_op = false;
			}
			if (select_part)
			{

				if (x >= 275 && x <= 475 && y >= 200 && y <= 500) // �κ� 1
				{
					player_attack = true;
					select_part = false;
					if (num <= Player_Attack_Percent[0]) {
						P_D = (float)((float)((rand() % (int)((Player_Damage[1] - Player_Damage[0]) * 20)) + (int)(Player_Damage[0] * 20)) / 10.0);
					}
				}
				else if (x >= 550 && x <= 750 && y >= 200 && y <= 500) // �κ� 2
				{
					player_attack = true;
					select_part = false;
					if (num <= Player_Attack_Percent[1])
						P_D = (float)((float)((rand() % (int)((Player_Damage[1] - Player_Damage[0]) * 70)) + (int)(Player_Damage[0] * 70)) / 100.0);
				}
				else if (x >= 850 && x <= 1050 && y >= 200 && y <= 500) // �κ� 3
				{
					player_attack = true;
					select_part = false;
					if (num <= Player_Attack_Percent[2])
						P_D = (float)((float)((rand() % (int)((Player_Damage[1] - Player_Damage[0]) * 10)) + (int)(Player_Damage[0] * 10)) / 10.0);
				}
				else if (x >= 1075 && x <= 1125 && y >= 50 && y <= 100) // �κ� 4
				{
					player_attack = false;
					enemy_attack = false;
					select_part = false;
				}
			}
			else
			{
				if (num <= Player_Attack_Percent[2])
					P_D = (float)((float)((rand() % (int)((Player_Damage[1] - Player_Damage[0]) * 10)) + (int)(Player_Damage[0] * 10)) / 10.0);

				if (player_room == 0) // ���� �濡�� ���� ����
				{
					if (x >= 140 && x < 290 && y >= screenRect.bottom / 2 - 20 && y < screenRect.bottom / 2 + 100)
					{
						player_attack = true;
						if (attack_op) select_part = true;
					}
				}
				else if (player_room == 1) // ��� �濡�� ���� ����
				{
					if (x >= screenRect.right / 2 && x < screenRect.right / 2 + 150 && y >= screenRect.bottom / 2 - 20 && y < screenRect.bottom / 2 + 100)
					{
						player_attack = true;
						if (attack_op) select_part = true;
					}
				}
				else if (player_room == 2) // ������ �濡�� ���� ����
				{
					if (x >= screenRect.right - 300 && x < screenRect.right - 150 && y >= screenRect.bottom / 2 - 20 && y < screenRect.bottom / 2 + 100)
					{
						player_attack = true;
						if (attack_op) select_part = true;
					}
				}
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
		img[3] = Image::FromFile(L"images/Player/Player_Fight.png");
		img[4] = Image::FromFile(L"images/Player/Player_Fight_Attack.png");
		img[5] = Image::FromFile(L"images/Player/Player_Fight_Weapon.png");
		img[6] = Image::FromFile(L"images/Player/Player_Fight_Weapon_Attack.png");
		img[7] = Image::FromFile(L"images/Player/Player_Damage.png");
		img[8] = Image::FromFile(L"images/Enemy/Enemy1_Idle.png");
		img[9] = Image::FromFile(L"images/Enemy/Enemy1_Damage.png");
		img[10] = Image::FromFile(L"images/Enemy/Enemy1_Attack.png");
		img[11] = Image::FromFile(L"images/Enemy/Enemy1_Death.png");
		img[12] = Image::FromFile(L"images/Enemy/EnemyHP.png");
		img[13] = Image::FromFile(L"images/Enemy/E1.png");
		img[14] = Image::FromFile(L"images/Enemy/E1_UI.png");
		img[15] = Image::FromFile(L"images/Player/Attack.png");
		img[16] = Image::FromFile(L"images/Player/Attack2.png");
		img[17] = Image::FromFile(L"images/Player/P(80,50).png");
		img[18] = Image::FromFile(L"images/Player/P(60,25).png");
		img[19] = Image::FromFile(L"images/Player/P(40,10).png");
		img[20] = Image::FromFile(L"images/Player/Bag.png");
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
		3. �� -> ĳ���� -> �� ������ �׷��ְ�, �湮���� ���� ���� ������ �簢������ ����������
		(�÷��̾ �̵��� �� ����� �������ų� ���� �������Ƿ� ��(��)�� ��ġ�� �°� �����簢�� �׷��ֱ�)
		4. ���� ��ġ�� ���� ������ �簢���� ����, �湮�ߴ� ���� ������ �簢���� ������

		=============||=============||=============
					 ||				||
					 ||				||
					 ||		 ��		||   
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
					 ||		 ��		||	
					 ||				||
					 ||				||
		=============||=============||=============

	*/



	SolidBrush brush(Color(255, 0, 0, 0));

	// ��Ʈ ����
	FontFamily fontFamily(L"Arial");
	Font font(&fontFamily, 10, FontStyleBold, UnitPixel);

	ImageAttributes ImgAttr, Shadow;
	ImgAttr.SetColorKey(Color(71, 71, 71), Color(71, 71, 71));
	Shadow.SetColorKey(Color(71, 71, 71), Color(71, 71, 71));

	ColorMatrix colorMatrix =
	{
		//	  R		G	  B     A
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	Shadow.SetColorMatrix(&colorMatrix);

	static int m_h = 20; // �� ���� ��ǥ
	//static int b_h = 0; // �� ���� ��ǥ
	static int p_w = 0, p_h = 318; // �÷��̾� �̹����� x ��ǥ, y��ǥ
	static int p_w2 = 0, p_w3 = 0, p_w4 = 0, p_w5 = 0, p_w6 = 0; // �÷��̾��� ���� �̹����� x, y ��ǥ
	static int e_h = 0; // ��ܹ濡 ���� ������ ��� �÷��̾�� �������� ���� �ö���� �ִϸ��̼��� ����
	static int e_w = 0, e_w2 = 0, e_w3 = 0, e_w4 = 0; // �� �̹������� x, y ��ǥ
	static int p_x = 135, p_x2 = 135; // �÷��̾ �׷��� x��ǥ
	static int p_y = 0, p_y2 = 0; // �÷��̾ �׷��� y��ǥ
	static int p_h2 = 636, p_h3 = 0; //p_h2 : ��� �������� ���� �ߺ��� ���̰� �ϱ� ����

	static bool left = false; // �÷��̾� ���� �̵� ������ �̵� üũ

	graphics.DrawImage(img[0], Rect(0, 0, screenRect.right, screenRect.bottom), 0, m_h, 1280, 700, UnitPixel);
	// UI �׸���
	graphics.DrawImage(img[1], Rect(10, 10, img[1]->GetWidth(), img[1]->GetHeight()), 0, 0, img[1]->GetWidth(), img[1]->GetHeight(), UnitPixel);
	img[17] = Image::FromFile(L"images/Player/P(80,50).png");
	img[18] = Image::FromFile(L"images/Player/P(60,25).png");
	img[19] = Image::FromFile(L"images/Player/P(40,10).png");
	if(Player_HP >= 80.0 && Player_Hunger >= 50)
		graphics.DrawImage(img[17], Rect(25, 20, img[17]->GetWidth(), img[17]->GetHeight()), 0, 0, img[17]->GetWidth(), img[17]->GetHeight(), UnitPixel);
	else if((Player_HP < 80.0 && Player_HP >= 60.0) || (Player_Hunger < 50 && Player_Hunger >= 25))
		graphics.DrawImage(img[18], Rect(25, 20, img[18]->GetWidth(), img[18]->GetHeight()), 0, 0, img[18]->GetWidth(), img[18]->GetHeight(), UnitPixel);
	else if((Player_HP < 60.0 && Player_HP >= 40.0) && Player_Hunger < 25)
		graphics.DrawImage(img[19], Rect(25, 20, img[19]->GetWidth(), img[19]->GetHeight()), 0, 0, img[19]->GetWidth(), img[19]->GetHeight(), UnitPixel);

	graphics.DrawImage(img[20], Rect(20, 630, img[20]->GetWidth(), img[20]->GetHeight()), 0, 0, img[20]->GetWidth(), img[20]->GetHeight(), UnitPixel);

	TCHAR P_HP[15], P_HUNG[10];
	wsprintf(P_HP, TEXT("%d.%d / 100.0"), (int)Player_HP, (int)(Player_HP * 10) % 10);
	wsprintf(P_HUNG, TEXT("%d / 100"), (int)Player_Hunger);
	// �ؽ�Ʈ �Է�
	brush.SetColor(Color(255, 255, 246, 202));

	PointF pointP_HP(70.5f, 23.0f), pointP_HUNG(87.5f, 65.5f);
	graphics.DrawString(P_HP, -1, &font, pointP_HP, &brush);
	graphics.DrawString(P_HUNG, -1, &font, pointP_HUNG, &brush);

	if (move_chk) // �÷��̾ ������
	{
		int dir = 0;
		if (direction.size() > 0)
		{
			dir = direction[0];
			switch (dir)
			{
				case 1: // ��
					{
						static bool upleft = true; // ����� �ö󰥶� �÷��̾ �������� ���� ���
						if(floor == 29)
						{
							if (m_h < 20)
							{
								upleft = true;
								direction.push_back(4);
								direction.erase(direction.begin() + 0);
								p_x = 210; p_x2 = 35;
								p_y = 0; p_y2 = 0;
								p_h2 = 636; p_h3 = 0;
								e_h = 0; e_w = 0;
								floor++;
								break;
							}
							else
							{
								if (!upleft) // ��� �ö󰡱� (���� -> ������)
								{
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20 + p_y, 100, 130 - p_y2), p_w, 954 - p_h2, 170, 318 - p_h3, UnitPixel, &Shadow);
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20 + p_y, 90, 120 - p_y2), p_w, 954 - p_h2, 170, 318 - p_h3, UnitPixel);
									p_x -= 4;
									if (p_x <= 35) upleft = true;
								}
								else if (upleft) // ������ -> ����
								{
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20 + p_y, 100, 130 - p_y2), p_w + 170, 954 - p_h2, -170, 318 - p_h3, UnitPixel, &Shadow);
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20 + p_y, 90, 120 - p_y2), p_w + 170, 954 - p_h2, -170, 318 - p_h3, UnitPixel);
									p_x += 3;
									if (p_x >= 25 && p_x <= 135)
									{
										p_h2 -= 16; p_h3 += 16;
										p_y += 6;  p_y2 += 6;
									}
									if (p_x >= 165) upleft = false;
								}
								if (p_y >= 130) p_y = 130;
								if (p_y2 >= 120) p_y2 = 120;
								if (p_h2 <= 318) p_h2 = 318;
								if (p_h3 >= 318) p_h3 = 318;
								m_h -= 3;
							}
						}
						else if (floor < 29)
						{
							if (m_h < 20 + (29 - floor) * 230)
							{
								upleft = true;
								direction.erase(direction.begin() + 0);
								p_x = 135;
								p_y = 0; p_y2 = 0;
								p_h2 = 636; p_h3 = 0;
								e_h = 0; e_w = 0;
								floor++;
								break;
							}
							else
							{
								if (!upleft) // ��� �ö󰡱� (���� -> ������)
								{
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30 + p_y, 100, 130 - p_y2), p_w, 954 - p_h2, 170, 318 - p_h3, UnitPixel, &Shadow);
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20 + p_y, 90, 120 - p_y2), p_w, 954 - p_h2, 170, 318 - p_h3, UnitPixel);
									p_x -= 4;
									if (p_x <= 35) upleft = true;
								}
								else if (upleft) // ������ -> ����
								{
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30 + p_y, 100, 130 - p_y2), p_w + 170, 954 - p_h2, -170, 318 - p_h3, UnitPixel, &Shadow);
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20 + p_y, 90, 120 - p_y2), p_w + 170, 954 - p_h2, -170, 318 - p_h3, UnitPixel);
									p_x += 3;
									if (p_x >= 25 && p_x <= 135)
									{
										p_h2 -= 16; p_h3 += 16;
										p_y += 6;  p_y2 += 6;
									}
									if (p_x >= 165) upleft = false;
								}
								if (p_y >= 130) p_y = 130;
								if (p_y2 >= 120) p_y2 = 120;
								if (p_h2 <= 318) p_h2 = 318;
								if (p_h3 >= 318) p_h3 = 318;
								m_h -= 3;
							}
						}
					}
					break;
				case 2: // �Ʒ�
					{
						if(floor == 30)
						{
							if (m_h >= 250)
							{
								left = false;
								direction.erase(direction.begin() + 0);
								p_x = 135; p_x2 = 35;
								p_y = 0; p_y2 = 0;
								p_h2 = 636; p_h3 = 0;
								e_h = 0; e_w = 0;
								floor--;
								break;
							}
							p_x2 += 5;
							if (p_x2 < 210) // �÷��̾ �������� ����� �̵��� ���
							{
								// ���� �̹��� ��� : graphics.DrawImage(img, 0, 0, 100, 100);
								// y�� ��Ī ��� : graphics.DrawImage(img, 100, 0, -100, 100);
								graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x2 + 10, screenRect.bottom / 2 - 30, 100, 130), p_w + 170, p_h, -170, 318, UnitPixel, &Shadow);
								graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x2, screenRect.bottom / 2 - 20, 90, 120), p_w + 170, p_h, -170, 318, UnitPixel);
							}
							else
							{
								if (!left) // ��� �������� (���� -> ������)
								{
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 + 100 - p_y, 100, p_y2 + 10), p_w, p_h2, 170, p_h3, UnitPixel, &Shadow);
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 + 110 - p_y, 90, p_y2), p_w, p_h2, 170, p_h3, UnitPixel);
									p_x -= 3;
									if (p_x <= 35) left = true;
								}
								else if (left) // ������ -> ����
								{
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 + 100 - p_y, 100, p_y2 + 10), p_w + 170, p_h2, -170, p_h3, UnitPixel, &Shadow);
									graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 + 110 - p_y, 90, p_y2), p_w + 170, p_h2, -170, p_h3, UnitPixel);
									p_x += 3;
									if (p_x >= 156) left = false;
								}
								p_h2 -= 8; p_h3 += 8;
								p_y += 3; p_y2 += 3;
								if (p_y >= 130) p_y = 130;
								if (p_y2 >= 120) p_y2 = 120;
								if (p_h2 <= 318) p_h2 = 318;
								if (p_h3 >= 318) p_h3 = 318;
								m_h += 3;
								if (e_h >= 250) e_h = 250;
								if (Enemy_room[floor - 1][player_room])
								{
									graphics.DrawImage(img[8], Rect(screenRect.right / 2 + 10, screenRect.bottom / 2 + 200 - e_h, 160, 130), e_w, 0, 200, 170, UnitPixel, &Shadow);
									graphics.DrawImage(img[8], Rect(screenRect.right / 2, screenRect.bottom / 2 + 210 - e_h, 150, 120), e_w, 0, 200, 170, UnitPixel, &ImgAttr);
									e_h += 3;
									e_w += 200;
								}
							}
						}
						else if (floor < 30)
						{
							if (m_h >= 250 + (30 - floor) * 230)
							{
								left = false;
								direction.erase(direction.begin() + 0);
								p_x = 135; p_x2 = 35;
								p_y = 0; p_y2 = 0;
								p_h2 = 636; p_h3 = 0;
								e_h = 0; e_w = 0;
								floor--;
								break;
							}
							if (!left) // ��� �������� (���� -> ������)
							{
								graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 + 100 - p_y, 100, p_y2 + 10), p_w, p_h2, 170, p_h3, UnitPixel, &Shadow);
								graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 + 110 - p_y, 90, p_y2), p_w, p_h2, 170, p_h3, UnitPixel);
								p_x -= 3;
								if (p_x <= 35) left = true;
							}
							else if (left) // ������ -> ����
							{
								graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 + 100 - p_y, 100, p_y2 + 10), p_w + 170, p_h2, -170, p_h3, UnitPixel, &Shadow);
								graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 + 110 - p_y, 90, p_y2), p_w + 170, p_h2, -170, p_h3, UnitPixel);
								p_x += 3;
								if (p_x >= 156) left = false;
							}
							p_h2 -= 8; p_h3 += 8;
							p_y += 3; p_y2 += 3;
							if (p_y >= 130) p_y = 130;
							if (p_y2 >= 120) p_y2 = 120;
							if (p_h2 <= 318) p_h2 = 318;
							if (p_h3 >= 318) p_h3 = 318;
							m_h += 3;
							if (e_h >= 230) e_h = 230;
							if (Enemy_room[floor - 1][player_room])
							{
								graphics.DrawImage(img[8], Rect(screenRect.right / 2 + 10, screenRect.bottom / 2 + 200 - e_h, 160, 130), e_w, 0, 200, 170, UnitPixel, &Shadow);
								graphics.DrawImage(img[8], Rect(screenRect.right / 2, screenRect.bottom / 2 + 210 - e_h, 150, 120), e_w, 0, 200, 170, UnitPixel, &ImgAttr);
								e_h += 3;
								e_w += 200;
							}
						}
						/*
							�� �׸���
						*/
					}
					break;
				case 3: // ����
					{
						if(room_chk[floor][1] == 2)
						{
							if (p_x >= 135)
							{
								direction.erase(direction.begin() + 0);
								left = false;
							}
							graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w + 170, p_h, -170, 318, UnitPixel, &Shadow);
							graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w + 170, p_h, -170, 318, UnitPixel);
							p_x += 5;
						}
						else
						{
							if (p_x >= 250)
							{
								direction.erase(direction.begin() + 0);
								left = true;
							}
							graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w + 170, p_h, -170, 318, UnitPixel, &Shadow);
							graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w + 170, p_h, -170, 318, UnitPixel);
							p_x += 5;
							if (Enemy_room[floor][player_room] && p_x >= 220)
							{
								graphics.DrawImage(img[8], Rect(screenRect.left + 150, screenRect.bottom / 2 - 30, 160, 130), e_w + 200, 0, -200, 170, UnitPixel, &Shadow);
								graphics.DrawImage(img[8], Rect(screenRect.left + 140, screenRect.bottom / 2 - 20, 150, 120), e_w + 200, 0, -200, 170, UnitPixel, &ImgAttr);
								e_w += 200;
							}
						}
					}
					break;
				case 4: // ������
					{
						if(room_chk[floor][1] == 2)
						{
							if (p_x <= 135)
							{
								direction.erase(direction.begin() + 0);
								left = false;
							}
							graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w, p_h, 170, 318, UnitPixel, &Shadow);
							graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w, p_h, 170, 318, UnitPixel);
							p_x -= 5;
						}
						else
						{
							if (p_x <= -180)
							{
								direction.erase(direction.begin() + 0);
								left = false;
							}
							graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w, p_h, 170, 318, UnitPixel, &Shadow);
							graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w, p_h, 170, 318, UnitPixel);
							p_x -= 5;
							if (Enemy_room[floor][player_room] && p_x <= -140)
							{
								graphics.DrawImage(img[8], Rect(screenRect.right - 290, screenRect.bottom / 2 - 30, 160, 130), e_w, 0, 200, 170, UnitPixel, &Shadow);
								graphics.DrawImage(img[8], Rect(screenRect.right - 300, screenRect.bottom / 2 - 20, 150, 120), e_w, 0, 200, 170, UnitPixel, &ImgAttr);
								e_w += 200;
							}
						}
					}
					break;
			}
		}
		p_w += 170;
		if (p_w >= 2210) p_w = 0;
		if (e_w >= 2600) e_w = 0;

		else if(direction.size() == 0)
		{
			move_chk = false;
			p_w = 0;
		}

	}
	else  // �÷��̾ ����
	{

		if (p_w >= 2210) p_w = 0;
		if (p_w2 >= 2340) p_w2 = 0;
		if (e_w >= 2600) e_w = 0;
		
		if (floor == 30)
		{
			graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 40, 100, 130), p_w, 0, 170, 318, UnitPixel, &Shadow);
			graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 30, 90, 120), p_w, 0, 170, 318, UnitPixel);
		}
		// 29�� ����
		else if (floor < 30 && floor > 1)
		{
			// �÷��̾ �ִ� ���� �濡 ���� �����Ѵٸ�
			if (Enemy_room[floor][player_room])
			{
				// �� HP bar
				graphics.DrawImage(img[12], Rect(screenRect.right - 170, screenRect.top, img[12]->GetWidth(), img[12]->GetHeight()), 0, 0, img[12]->GetWidth(), img[12]->GetHeight(), UnitPixel);
				graphics.DrawImage(img[13], Rect(screenRect.right - 85, screenRect.top + 25, img[13]->GetWidth(), img[13]->GetHeight()), 0, 0, img[13]->GetWidth(), img[13]->GetHeight(), UnitPixel);
				// �� HP ���
				TCHAR E_HP[25];
				wsprintf(E_HP, TEXT("%d.%d / %d.%d"), (int)Enemy_HP[floor][player_room], (int)(Enemy_HP[floor][player_room] * 10) % 10, (int)Enemy_MaxHP, (int)(Enemy_MaxHP * 10) % 10);

				// �ؽ�Ʈ �Է�
				brush.SetColor(Color(255, 255, 246, 202));

				PointF pointE_HP(screenRect.right - 143.5f, 68.5f);
				graphics.DrawString(E_HP, -1, &font, pointE_HP, &brush);
				// ���� ���� �� UI
				if (!attack_op) graphics.DrawImage(img[15], Rect(screenRect.right / 2 - 50, screenRect.bottom - 70, 100, 60), 0, 0, img[15]->GetWidth(), img[15]->GetHeight(), UnitPixel);
				else graphics.DrawImage(img[16], Rect(screenRect.right / 2 - 50, screenRect.bottom - 110, 100, 100), 0, 0, img[16]->GetWidth(), img[16]->GetHeight(), UnitPixel);

				// Ư�� ������ �����ϴ� �ɼ��� Ű�� ���� Ŭ���� ���
				if (attack_op && select_part)
				{
					// >> : ��� ��Ӱ�
					brush.SetColor(Color(225, 0, 0, 0));
					graphics.FillRectangle(&brush, 0, 0, screenRect.right, screenRect.bottom);
					// <<
					graphics.DrawImage(img[14], Rect(220, 50, img[14]->GetWidth(), img[14]->GetHeight()), 0, 0, img[14]->GetWidth(), img[14]->GetHeight(), UnitPixel);
					Font font(&fontFamily, 20, FontStyleBold, UnitPixel);
					// Ȯ��, ������ �� ���
					TCHAR Per1[4], Per2[4], Per3[4]; // �⺻ 50%, 90%, 80%
					TCHAR Dam1[30], Dam2[30], Dam3[30]; // �⺻ Damage * 2, Damage * 0.7, Damage * 1
					wsprintf(Per1, TEXT("%d%%"), Player_Attack_Percent[0]);
					wsprintf(Per2, TEXT("%d%%"), Player_Attack_Percent[1]);
					wsprintf(Per3, TEXT("%d%%"), Player_Attack_Percent[2]);
					wsprintf(Dam1, TEXT("������ : %d.%d - %d.%d"), (int)Player_Damage[0] * 2, (int)(Player_Damage[0] * 20) % 10, (int)Player_Damage[1] * 2, (int)(Player_Damage[1] * 20) % 10);
					wsprintf(Dam2, TEXT("������ : %d.%d - %d.%d"), (int)(Player_Damage[0] * 7) / 10, (int)(Player_Damage[0] * 7) % 10, (int)(Player_Damage[1] * 7) / 10, (int)(Player_Damage[1] * 7) % 10);
					wsprintf(Dam3, TEXT("������ : %d.%d - %d.%d"), (int)Player_Damage[0], (int)(Player_Damage[0] * 10) % 10, (int)Player_Damage[1], (int)(Player_Damage[1] * 10) % 10);

					// �ؽ�Ʈ �Է�
					brush.SetColor(Color(255, 161, 156, 129));

					PointF pointF1(373.0f, 520.0f), pointF2(648.0f, 520.0f), pointF3(948.0f, 520.0f);
					graphics.DrawString(Per1, -1, &font, pointF1, &brush);
					graphics.DrawString(Per2, -1, &font, pointF2, &brush);
					graphics.DrawString(Per3, -1, &font, pointF3, &brush);

					// 
					brush.SetColor(Color(255, 183, 50, 50));
					PointF pointF4(300.0f, 550.0f), pointF5(575.0f, 550.0f), pointF6(875.0f, 550.0f);
					graphics.DrawString(Dam1, -1, &font, pointF4, &brush);
					graphics.DrawString(Dam2, -1, &font, pointF5, &brush);
					graphics.DrawString(Dam3, -1, &font, pointF6, &brush);
				}
				// Ư�� ������ �����ϴ� �ɼ��� �� ���·� ���� Ŭ���ϰų� �ɼ��� Ű�� ���� Ŭ���ϰ� Ư�� ������ ������ ���
				else
				{
					if (player_attack || enemy_attack) // �÷��̾� ���� �Ǵ� ���� ����
					{
						Font font(&fontFamily, 30, FontStyleBold, UnitPixel);
						brush.SetColor(Color(255, 255, 0, 0));
						static int t_h = -50;
						TCHAR P_Dam[6], E_Dam[6], Miss[5];
						wsprintf(P_Dam, TEXT("%d.%d"), (int)P_D, (int)(P_D * 10) % 10);
						wsprintf(E_Dam, TEXT("%d.%d"), (int)E_D, (int)(E_D * 10) % 10);
						wsprintf(Miss, TEXT("��ħ"));
						if (player_room == 0) // ���� �濡 �ִ� ���
						{
							if (player_attack && !enemy_attack) // �÷��̾��� ���� ������ ���
							{
								graphics.DrawImage(img[4], Rect(screenRect.right / 2 - (p_x + 40), screenRect.bottom / 2 - 30, 130, 130), p_w3 + 249, 0, -249, 300, UnitPixel, &Shadow);
								graphics.DrawImage(img[4], Rect(screenRect.right / 2 - (p_x + 30), screenRect.bottom / 2 - 20, 120, 120), p_w3 + 249, 0, -249, 300, UnitPixel);
								graphics.DrawImage(img[9], Rect(screenRect.left + 150, screenRect.bottom / 2 - 40, 210, 160), e_w2 + 208, 0, -208, 238, UnitPixel, &Shadow);
								graphics.DrawImage(img[9], Rect(screenRect.left + 140, screenRect.bottom / 2 - 50, 200, 150), e_w2 + 208, 0, -208, 238, UnitPixel, &ImgAttr);
								PointF pointF(screenRect.left + 250, screenRect.bottom / 2 + t_h);
								if(P_D > 0.0) graphics.DrawString(P_Dam, -1, &font, pointF, &brush);
								else graphics.DrawString(Miss, -1, &font, pointF, &brush);
								p_w3 += 249;
								e_w2 += 208;
								t_h -= 3;
								if (p_w3 == 249) sound.PlaySoundEffect(2);
								if (e_w2 >= 2288 || p_w3 >= 2739) {
									sound.PlaySoundEffect(3);
									p_w3 = 0;
									e_w2 = 0;
									t_h = -50;
									player_attack = false;
									Enemy_HP[floor][player_room] -= P_D;
									if (Enemy_HP[floor][player_room] > 0.0)
										enemy_attack = true;
									else if(Enemy_HP[floor][player_room] <= 0.0)
									{
										sound.PlaySoundEffect(5);
										Enemy_Death[floor][player_room] = true;
										Enemy_room[floor][player_room] = false;
										Player_HP += rand() % 5 + 6;
										Player_Exp += Enemy_EXP;
									}
								}
							}
							else if (enemy_attack && !player_attack) // ���� ���� ������ ���
							{
								if (e_w3 >= 3367) {
									p_w6 = 0;
									e_w3 = 0;
									t_h = -50;
									enemy_attack = false;
									Player_HP -= E_D;
								}
								graphics.DrawImage(img[7], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w6 + 149, 0, -149, 295, UnitPixel, &Shadow);
								graphics.DrawImage(img[7], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w6 + 149, 0, -149, 295, UnitPixel);
								graphics.DrawImage(img[10], Rect(screenRect.left + 150, screenRect.bottom / 2 - 80, 269, 180), e_w3 + 259, 0, -259, 304, UnitPixel, &Shadow);
								graphics.DrawImage(img[10], Rect(screenRect.left + 140, screenRect.bottom / 2 - 70, 259, 170), e_w3 + 259, 0, -259, 304, UnitPixel, &ImgAttr);
								PointF pointF(screenRect.right / 2 - p_x, screenRect.bottom / 2 + t_h);
								if (E_D > 0.0) graphics.DrawString(E_Dam, -1, &font, pointF, &brush);
								else graphics.DrawString(Miss, -1, &font, pointF, &brush);
								t_h -= 3;
								p_w6 += 149;
								if (p_w6 >= 1043) p_w6 = 894;
								e_w3 += 259;
							}
						}
						else if (player_room == 1) // ��ܹ濡 �ִ� ���
						{
							if (player_attack && !enemy_attack) // �÷��̾��� ���� ������ ���
							{

								graphics.DrawImage(img[4], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 130, 130), p_w3, 0, 249, 300, UnitPixel, &Shadow);
								graphics.DrawImage(img[4], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 120, 120), p_w3, 0, 249, 300, UnitPixel);
								graphics.DrawImage(img[9], Rect(screenRect.right / 2 - 5, screenRect.bottom / 2 - 60, 210, 160), e_w2, 0, 208, 238, UnitPixel, &Shadow);
								graphics.DrawImage(img[9], Rect(screenRect.right / 2 - 15, screenRect.bottom / 2 - 50, 200, 150), e_w2, 0, 208, 238, UnitPixel, &ImgAttr);
								PointF pointF(screenRect.right / 2 - 15, screenRect.bottom / 2 + t_h);
								if (P_D > 0.0) graphics.DrawString(P_Dam, -1, &font, pointF, &brush);
								else graphics.DrawString(Miss, -1, &font, pointF, &brush);
								p_w3 += 249;
								e_w2 += 208;
								t_h -= 3;
								if (p_w3 == 249) sound.PlaySoundEffect(2);
								if (e_w2 >= 2288 || p_w3 >= 2739) {
									sound.PlaySoundEffect(3);
									p_w3 = 0;
									e_w2 = 0;
									t_h = -50;
									player_attack = false;
									Enemy_HP[floor][player_room] -= P_D;
									if (Enemy_HP[floor][player_room] > 0.0)
										enemy_attack = true;
									else if (Enemy_HP[floor][player_room] <= 0.0)
									{
										sound.PlaySoundEffect(5);
										Enemy_Death[floor][player_room] = true;
										Enemy_room[floor][player_room] = false;
										Player_HP += rand() % 5 + 6;
										Player_Exp += Enemy_EXP;
									}
								}
							}
							else if (enemy_attack && !player_attack) // ���� ���� ������ ���
							{
								if (e_w3 >= 3367) {
									p_w6 = 0;
									e_w3 = 0;
									t_h = -120;
									enemy_attack = false;
									Player_HP -= E_D;
								}
								graphics.DrawImage(img[7], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w6, 0, 149, 295, UnitPixel, &Shadow);
								graphics.DrawImage(img[7], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w6, 0, 149, 295, UnitPixel);
								graphics.DrawImage(img[10], Rect(screenRect.right / 2 - 90, screenRect.bottom / 2 - 80, 269, 180), e_w3, 0, 259, 304, UnitPixel, &Shadow);
								graphics.DrawImage(img[10], Rect(screenRect.right / 2 - 100, screenRect.bottom / 2 - 70, 259, 170), e_w3, 0, 259, 304, UnitPixel, &ImgAttr);
								p_w6 += 149;
								PointF pointF(screenRect.right / 2 - p_x, screenRect.bottom / 2 + t_h);
								if (E_D > 0.0) graphics.DrawString(E_Dam, -1, &font, pointF, &brush);
								else graphics.DrawString(Miss, -1, &font, pointF, &brush);
								t_h -= 3; 
								if (p_w6 >= 1043) p_w6 = 894;
								e_w3 += 259;
							}
						}
						else if(player_room == 2) // �����ʹ濡 �ִ� ���
						{
							if (player_attack && !enemy_attack) // �÷��̾��� ���� ������ ���
							{

								graphics.DrawImage(img[4], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 130, 130), p_w3, 0, 249, 300, UnitPixel, &Shadow);
								graphics.DrawImage(img[4], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 120, 120), p_w3, 0, 249, 300, UnitPixel);
								graphics.DrawImage(img[9], Rect(screenRect.right - 305, screenRect.bottom / 2 - 60, 210, 160), e_w2, 0, 208, 238, UnitPixel, &Shadow);
								graphics.DrawImage(img[9], Rect(screenRect.right - 315, screenRect.bottom / 2 - 50, 200, 150), e_w2, 0, 208, 238, UnitPixel, &ImgAttr);
								PointF pointF(screenRect.right - 315, screenRect.bottom / 2 + t_h);
								if (P_D > 0.0) graphics.DrawString(P_Dam, -1, &font, pointF, &brush);
								else graphics.DrawString(Miss, -1, &font, pointF, &brush);
								p_w3 += 249;
								e_w2 += 208;
								t_h -= 3;
								if (p_w3 == 249) sound.PlaySoundEffect(2);
								if (e_w2 >= 2288 || p_w3 >= 2739) {
									sound.PlaySoundEffect(3);
									p_w3 = 0;
									e_w2 = 0;
									t_h = -50;
									player_attack = false;
									Enemy_HP[floor][player_room] -= P_D;
									if (Enemy_HP[floor][player_room] > 0.0)
										enemy_attack = true;
									else if (Enemy_HP[floor][player_room] <= 0.0)
									{
										sound.PlaySoundEffect(5);
										Enemy_Death[floor][player_room] = true;
										Enemy_room[floor][player_room] = false;
										Player_HP += rand() % 5 + 6;
										Player_Exp += Enemy_EXP;
									}
								}
							}
							else if (enemy_attack && !player_attack) // ���� ���� ������ ���
							{
								if (e_w3 >= 3367) {
									p_w6 = 0;
									e_w3 = 0;
									t_h = -50;
									enemy_attack = false;
									Player_HP -= E_D;
									sound.PlaySoundEffect(3);
								}
								graphics.DrawImage(img[7], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w6, 0, 149, 295, UnitPixel, &Shadow);
								graphics.DrawImage(img[7], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w6, 0, 149, 295, UnitPixel);
								graphics.DrawImage(img[10], Rect(screenRect.right - 390, screenRect.bottom / 2 - 80, 269, 180), e_w3, 0, 259, 304, UnitPixel, &Shadow);
								graphics.DrawImage(img[10], Rect(screenRect.right - 400, screenRect.bottom / 2 - 70, 259, 170), e_w3, 0, 259, 304, UnitPixel, &ImgAttr);
								PointF pointF(screenRect.right / 2 - p_x, screenRect.bottom / 2 + t_h);
								if (E_D > 0.0) graphics.DrawString(E_Dam, -1, &font, pointF, &brush);
								else graphics.DrawString(Miss, -1, &font, pointF, &brush);
								t_h -= 3; 
								p_w6 += 149;
								if (p_w6 >= 1043) p_w6 = 894;
								e_w3 += 259;
							}
						}
					}
					// ������ �ϰ����� ���� ����
					else
					{
						//if (e_w == 0) sound.PlaySoundEffect(4);
						if (player_room == 0) // ���� �濡 �ִ� ���
						{
							graphics.DrawImage(img[3], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w2 + 180, 0, -180, 300, UnitPixel, &Shadow);
							graphics.DrawImage(img[3], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w2 + 180, 0, -180, 300, UnitPixel);
							graphics.DrawImage(img[8], Rect(screenRect.left + 150, screenRect.bottom / 2 - 30, 160, 130), e_w + 200, 0, -200, 170, UnitPixel, &Shadow);
							graphics.DrawImage(img[8], Rect(screenRect.left + 140, screenRect.bottom / 2 - 20, 150, 120), e_w + 200, 0, -200, 170, UnitPixel, &ImgAttr);
						}
						else if (player_room == 1)
						{
							graphics.DrawImage(img[3], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w2, 0, 180, 300, UnitPixel, &Shadow);
							graphics.DrawImage(img[3], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w2, 0, 180, 300, UnitPixel);
							graphics.DrawImage(img[8], Rect(screenRect.right / 2 + 10, screenRect.bottom / 2 - 30, 160, 130), e_w, 0, 200, 170, UnitPixel, &Shadow);
							graphics.DrawImage(img[8], Rect(screenRect.right / 2 , screenRect.bottom / 2 - 20, 150, 120), e_w, 0, 200, 170, UnitPixel, &ImgAttr);
						}
						else if(player_room == 2) // ������ �濡 �ִ� ���
						{
							graphics.DrawImage(img[3], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w2, 0, 180, 300, UnitPixel, &Shadow);
							graphics.DrawImage(img[3], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w2, 0, 180, 300, UnitPixel);
							graphics.DrawImage(img[8], Rect(screenRect.right - 290, screenRect.bottom / 2 - 30, 160, 130), e_w, 0, 200, 170, UnitPixel, &Shadow);
							graphics.DrawImage(img[8], Rect(screenRect.right - 300, screenRect.bottom / 2 - 20, 150, 120), e_w, 0, 200, 170, UnitPixel, &ImgAttr);
						}
						p_w2 += 180;
						e_w += 200;
					}
				}
			}
			// ���� ������ ���� ��
			else
			{
				static int d_w = 0;
				if (player_room == 0)
				{
					graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w + 170, 0, -170, 318, UnitPixel, &Shadow);
					graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w + 170, 0, -170, 318, UnitPixel);
					if (!Enemy_room[floor][player_room] && Enemy_Death[floor][player_room])
					{
						graphics.DrawImage(img[11], Rect(screenRect.left + 150, screenRect.bottom / 2 - 30, 160, 130), d_w, 0, 295, 262, UnitPixel, &Shadow);
						graphics.DrawImage(img[11], Rect(screenRect.left + 140, screenRect.bottom / 2 - 20, 150, 120), d_w, 0, 295, 262, UnitPixel);
						d_w += 295;
					}
				}
				else if (player_room == 1)
				{
					graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w, 0, 170, 318, UnitPixel, &Shadow);
					graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w, 0, 170, 318, UnitPixel);
					if (!Enemy_room[floor][player_room] && Enemy_Death[floor][player_room])
					{
						graphics.DrawImage(img[11], Rect(screenRect.right / 2 + 10, screenRect.bottom / 2 - 30, 160, 130), d_w, 0, 295, 262, UnitPixel, &Shadow);
						graphics.DrawImage(img[11], Rect(screenRect.right / 2, screenRect.bottom / 2 - 20, 150, 120), d_w, 0, 295, 262, UnitPixel);
						d_w += 295;
					}
				}
				else if (player_room == 2)
				{
					graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x + 10, screenRect.bottom / 2 - 30, 100, 130), p_w, 0, 170, 318, UnitPixel, &Shadow);
					graphics.DrawImage(img[2], Rect(screenRect.right / 2 - p_x, screenRect.bottom / 2 - 20, 90, 120), p_w, 0, 170, 318, UnitPixel);
					if (!Enemy_room[floor][player_room] && Enemy_Death[floor][player_room])
					{
						graphics.DrawImage(img[11], Rect(screenRect.right - 290, screenRect.bottom / 2 - 30, 160, 130), d_w, 0, 295, 262, UnitPixel, &Shadow);
						graphics.DrawImage(img[11], Rect(screenRect.right - 300, screenRect.bottom / 2 - 20, 150, 120), d_w, 0, 295, 262, UnitPixel);
						d_w += 295;
					}
				}
				if (d_w >= 2655) d_w = 2655;
			}
		}
		// 1���� ���
		else
		{

		}
		p_w += 170;
	}
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


