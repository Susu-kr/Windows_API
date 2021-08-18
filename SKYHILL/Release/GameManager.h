#pragma once

#include "framework.h"
#include "Sound.h"
enum STATE { START, INGAME , CLEAR, OVER, END };	// ���� ���� ����
enum DIFF { EASY, NORMAL, HARD };					// ���� ���̵�
class GameManager
{
	UINT nFlags;
	ULONG_PTR			g_GdiToken;					
	int					state;
	int					difficulty;
	HBITMAP				hDoubleBufferImage;
	CSound				sound;
	Image*				img[22];					// img �迭
	bool				chk_op;						// �ɼ� ��ư�� ���� ���
	bool				chk_game;					// ���� ���� ��ư�� ���� ���
	bool				chk_ability;				// ���� ���̵��� ������ ��
	int					floor;						// ���� �� ��
	int					room_chk[31][3] = { 0 };	// 1������ 30������ �� ĭ�� �湮 Ȯ�� 0 : �湮x, 1 : �湮o, 2 : ���� ��ġ
	bool				move_chk;					// ������
	vector<int>			direction;					// 1 : ��, 2 : �Ʒ�, 3 : ����, 4 : ������
	bool				attack_op;					// Ư�� ���� ���� �ɼ�
	bool				select_part;				// Ư�� ���� ����
	bool				player_attack;				// �÷��̾� ���� ����
	bool				enemy_attack;				// �� ���� ����
	bool				passive[4] = { false };		// 0 : ���� , 1 : ���� óġ, 2 : ���� ŰƮ, 3 : �� ���� ��
	bool				active[4] = { false };		// 0 : ����, 1 : ���� �¿��ϴ� ����, 2 : ������â 3 : ��Ұ�����

	// << : Player
	float				Player_HP;						 // ü��
	float				Player_MaxHP = 100.0;			 // �ִ� ü��
	int					Player_Hunger;					 // �����
	int					Player_MaxHunger = 100;			 // �ִ� �����
	int					Player_Stat[4] = { 5, 5, 5, 5 }; // ���� = STR, SPD, DEX, ACC
	int					Player_Exp = 0;					 // ����ġ
	int					Player_NextExp = 50;			 // 1 -> 2������ �������� ����ġ
	int					Player_Level = 1;				 // ���� ����
	float				Player_Damage[2];				 // �������� �����Ҷ� 0 : �ּҵ�����, 1 : �ִ뵥����
	int					Player_Attack_Percent[3];		 // ������ Ȯ�� 0 : �Ӹ�(50%), 1 : ��(90%), 2 : ����(80%)
	int					player_room = 1;				 // �÷��̾ ��ġ�� ��
	float				P_D;							 // �÷��̾� -> �� ������

	// << : Enemy
	float				Enemy_HP[31][3];				 // �� �渶�� ���� HP
	float				Enemy_MaxHP = 15.7;				 // �ִ� �� HP
	float				Enemy_Damage[2];				 // 0 : ���� �ּ� ������ , 1 : ���� �ִ� ������ (���̵��� ���� �ٸ�)
	int					Enemy_EXP;						 // ���� ���� ��� ����ġ (���̵��� ���� �ٸ���.)
	bool				Enemy_room[31][3] = { false };	 // ���� �����ϴ� ��
	bool				Enemy_Death[31][3] = { false };	 // ���� ���� ��
	float				E_D;							 // �� -> �÷��̾� ������


public:
	RECT				screenRect;
	void				Init();											// �ʱ�ȭ
	void				GameReset();									// ���� �����
	void				Update();										// ������Ʈ
	void				DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);	// ������۸�
	void				Transition(int _state);							// state�� ���� ���� ����
	void				GDI_Init();
	void				GDI_Draw(HDC hdc);
	void				GDI_End();

	bool				Click(float x, float y);						// ���콺�� Ŭ���� ���콺�� ��ǥ�� �޾ƿ� ����

	void loadImage(int s);						// state�� �´� ȭ���� �̹������� �迭�� ����
	int getState() { return state; }			
	int getDifficulty() { return difficulty; }
	void DrawStartScreen(Graphics& graphics);	// state = START
	void DrawGameScreen(Graphics& graphics);	// state = INGAME
	void DrawClearScreen(Graphics& graphics);	// state = CLEAR
	void DrawOverScreen(Graphics& graphics);	// state = OVER
	void DrawEndScreen(Graphics& graphics);		// state = END
};
