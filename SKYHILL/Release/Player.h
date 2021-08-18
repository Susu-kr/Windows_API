#pragma once
#include "framework.h"

class Player
{
	bool				passive[4]; // 0 : 없음 , 1 : 응급 처치, 2 : 생존 키트, 3 : 운 좋은 놈
	bool				active[4]; // 0 : 없음, 1 : 운이 좌우하는 게임, 2 : 엉망진창 3 : 고소공포증
	float				HP;
	float				Hunger;
	int					Exp;
	int					Level;

	bool				Weapon[2];
	bool				Item[2];

};

