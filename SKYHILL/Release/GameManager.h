#pragma once

#include "framework.h"
#include "Sound.h"
enum STATE { START, INGAME , CLEAR, OVER, END };	// 현재 게임 상태
enum DIFF { EASY, NORMAL, HARD };					// 게임 난이도
class GameManager
{
	UINT nFlags;
	ULONG_PTR			g_GdiToken;					
	int					state;
	int					difficulty;
	HBITMAP				hDoubleBufferImage;
	CSound				sound;
	Image*				img[22];					// img 배열
	bool				chk_op;						// 옵션 버튼을 누른 경우
	bool				chk_game;					// 게임 시작 버튼을 누른 경우
	bool				chk_ability;				// 게임 난이도를 설정한 후
	int					floor;						// 현재 층 수
	int					room_chk[31][3] = { 0 };	// 1층부터 30층까지 방 칸수 방문 확인 0 : 방문x, 1 : 방문o, 2 : 현재 위치
	bool				move_chk;					// 움직임
	vector<int>			direction;					// 1 : 위, 2 : 아래, 3 : 왼쪽, 4 : 오른쪽
	bool				attack_op;					// 특정 부위 공격 옵션
	bool				select_part;				// 특정 부위 선택
	bool				player_attack;				// 플레이어 공격 순서
	bool				enemy_attack;				// 적 공격 순서
	bool				passive[4] = { false };		// 0 : 없음 , 1 : 응급 처치, 2 : 생존 키트, 3 : 운 좋은 놈
	bool				active[4] = { false };		// 0 : 없음, 1 : 운이 좌우하는 게임, 2 : 엉망진창 3 : 고소공포증

	// << : Player
	float				Player_HP;						 // 체력
	float				Player_MaxHP = 100.0;			 // 최대 체력
	int					Player_Hunger;					 // 배고픔
	int					Player_MaxHunger = 100;			 // 최대 배고픔
	int					Player_Stat[4] = { 5, 5, 5, 5 }; // 스탯 = STR, SPD, DEX, ACC
	int					Player_Exp = 0;					 // 경험치
	int					Player_NextExp = 50;			 // 1 -> 2레벨로 가기위한 경험치
	int					Player_Level = 1;				 // 현재 레벨
	float				Player_Damage[2];				 // 부위별로 공격할때 0 : 최소데미지, 1 : 최대데미지
	int					Player_Attack_Percent[3];		 // 부위별 확률 0 : 머리(50%), 1 : 팔(90%), 2 : 몸통(80%)
	int					player_room = 1;				 // 플레이어가 위치한 방
	float				P_D;							 // 플레이어 -> 적 데미지

	// << : Enemy
	float				Enemy_HP[31][3];				 // 각 방마다 적의 HP
	float				Enemy_MaxHP = 15.7;				 // 최대 적 HP
	float				Enemy_Damage[2];				 // 0 : 적의 최소 데미지 , 1 : 적의 최대 데미지 (난이도에 따라 다름)
	int					Enemy_EXP;						 // 적을 잡을 경우 경험치 (난이도에 따라 다르다.)
	bool				Enemy_room[31][3] = { false };	 // 적이 존재하는 방
	bool				Enemy_Death[31][3] = { false };	 // 적이 죽은 방
	float				E_D;							 // 적 -> 플레이어 데미지


public:
	RECT				screenRect;
	void				Init();											// 초기화
	void				GameReset();									// 게임 재시작
	void				Update();										// 업데이트
	void				DrawBitmapDoubleBuffering(HWND hWnd, HDC hdc);	// 더블버퍼링
	void				Transition(int _state);							// state에 따른 게임 상태
	void				GDI_Init();
	void				GDI_Draw(HDC hdc);
	void				GDI_End();

	bool				Click(float x, float y);						// 마우스로 클릭시 마우스의 좌표를 받아와 진행

	void loadImage(int s);						// state에 맞는 화면의 이미지들을 배열에 저장
	int getState() { return state; }			
	int getDifficulty() { return difficulty; }
	void DrawStartScreen(Graphics& graphics);	// state = START
	void DrawGameScreen(Graphics& graphics);	// state = INGAME
	void DrawClearScreen(Graphics& graphics);	// state = CLEAR
	void DrawOverScreen(Graphics& graphics);	// state = OVER
	void DrawEndScreen(Graphics& graphics);		// state = END
};
