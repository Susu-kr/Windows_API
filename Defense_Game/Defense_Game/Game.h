#pragma once
#ifndef GAME_H_
#define GAME_H_
#define _USE_MATH_DEFINES
#include "framework.h"
#include "Defense_Game.h"
#include <cmath>

enum MODE { LOBBY, INGAME, ENDGAME };

struct POS {
	float x;
	float y;
};

class GAME {
protected:
	POS		center;
	float	size;
	float	rotate;
	int		Block_Life;
	DWORD	score;
	MODE	mode;

public:
	GAME() : center({ 0, 0 }), size(0), Block_Life(3), mode(LOBBY) {}
	
	// Get
	POS		GetCenter() { return center; }
	float	GetSize()	{ return size; }
	int		GetB_Life() { return Block_Life; }
	MODE	GetMode()	{ return mode; }
	DWORD	GetScore()	{ return score; }

	void	SetMode(MODE m) { mode = m; }
	
	void Lobby(HDC hdc, RECT & R, TCHAR *str, SIZE temp);
	
	void InGame();
	void EndGame(HDC hdc, RECT & R, TCHAR * P);
};

#endif // !GAME_H_

