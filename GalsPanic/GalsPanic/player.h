#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "framework.h"

using namespace std;

struct polyLine {
	int		startVertex;
	int		endVertex;
	double	slope;
	int		polyState;
};

class player
{
	vector<POINT>	MovePt;
	POINT			position, ptVector;
	int				speed, radius;
	bool			collidState;
	bool			isInLine;
	bool			isVertex;
	bool			isVertex2;
	bool			clockWise;
	LONG			areaSize;

public:
	player() : ptVector({ 0, 0 }), speed(10), radius(10), collidState(false),
		isInLine(false), isVertex(false), isVertex2(false) {}
	~player();

};

// https://github.com/asdf-cmyk/winapi_galsPanic/blob/master/movingPoint.cpp