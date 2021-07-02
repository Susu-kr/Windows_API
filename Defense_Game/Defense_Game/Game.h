#pragma once
#ifndef GAME_H_
#define GAME_H_
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include "framework.h"
#include "Defense_Game.h"
#include "Object.h"

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <time.h>

using namespace std;

class GAME {
private:
	TCHAR	ID[15]	= { 0 };
	int		Score	= 0;
	int		HP		= 0;
	PLAY	*cannon;
	vector	<Wall*>		wall;
	vector	<Bullet*>	bullet;
	vector	<Enemy*>	enemy;

public:
	GAME() { memset(ID, 0, sizeof(TCHAR) * 15);	}
	~GAME();
	
	TCHAR * Get_ID() { return ID; }
	int		Get_HP() { return HP; }
	void	LOBBY		(HDC & hdc, RECT & R, SIZE & size);
	void	INGAME		(HDC & hdc, RECT & R);
	void	ENDGAME		(HDC & hdc, RECT & R, TCHAR name[][15], int *score);
	void	Update		(RECT);

	void	Create_Cannon		(RECT & R, FLOAT rad);
	void	Create_Wall			(RECT & R, FLOAT w, FLOAT h);
	void	Create_Enemy		(RECT & R, FLOAT rad);

	void	Draw_Cannon		(HDC hdc);
	void	Draw_Wall		(HDC hdc);
	void	Draw_Bullet		(HDC hdc, int);
	void	Draw_Enemy		(HDC hdc, int);

	void	Move_Cannon		(bool);
	void	Shoot_Cannon	(FLOAT);

	void	Save			(TCHAR(*name)[15], int *score);
};

#endif // !GAME_H_

