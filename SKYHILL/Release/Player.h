#pragma once
#include "framework.h"

class Player
{
	bool				passive[4]; // 0 : ���� , 1 : ���� óġ, 2 : ���� ŰƮ, 3 : �� ���� ��
	bool				active[4]; // 0 : ����, 1 : ���� �¿��ϴ� ����, 2 : ������â 3 : ��Ұ�����
	float				HP;
	float				Hunger;
	int					Exp;
	int					Level;

	bool				Weapon[2];
	bool				Item[2];

};

