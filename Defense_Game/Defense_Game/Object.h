#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_
#define _USE_MATH_DEFINES
#include "framework.h"
#include "Defense_Game.h"
#include <cmath>

struct Pos {
	FLOAT x;
	FLOAT y;
};

class STRUCT {
protected:
	Pos		center;
	bool	Active;
public:
	STRUCT(FLOAT x, FLOAT y) : center({ x, y }) {}

	Pos		GetCenter() { return center; }
	bool	GetActive() { return Active; }

	void	SetCenter(FLOAT x, FLOAT y) { center = { x, y }; }
	void	SetActive(bool a)			{ Active = a; }

	virtual bool Collision(STRUCT * a) { return false; }	// 충돌 여부 확인
};

class PLAY : public STRUCT 
{
private:
	FLOAT radian = M_PI * 0.5;
	FLOAT scale;
public:
	PLAY(FLOAT x, FLOAT y, FLOAT r) : STRUCT(x, y), scale(r) {}
	
	FLOAT GetScale()	{ return scale; }
	FLOAT GetRadian()	{ return radian; }

	void SetScale(FLOAT s) { scale = s; }
};

class Bullet : public STRUCT
{
private:
	Pos		POS;
	FLOAT	scale;
public:
	Bullet(float x, float y, FLOAT r, FLOAT radian) : STRUCT(x, y), scale(r) {
		POS.x = 5 * cos(radian);
		POS.y = 5 * sin(radian);
	}
	Pos		GetPOS()	{ return POS; }
	FLOAT	GetScale()	{ return scale; }
	bool Collision(STRUCT * a);
};

class Wall : public STRUCT
{
private:
	FLOAT width;
	FLOAT height;
public:
	Wall(FLOAT x, FLOAT y, FLOAT w, FLOAT h) : STRUCT(x, y), width(w), height(h) {}
	FLOAT	GetWidth()	{ return width; }
	FLOAT	GetHeight() { return height; }
};

class Enemy : public STRUCT
{
private:
	FLOAT spin;
	FLOAT rotate;
	FLOAT degree;
	FLOAT scale;
	POINT star[10];
	Pos point[5];
	Pos dup_p[5];

public:
	Enemy(FLOAT x, FLOAT y);

	Pos intersection(const Pos* p1, const Pos* p2, const Pos* p3, const Pos* p4);
	void SetStar(float spin);

	bool Collision(STRUCT * a);
	virtual void Draw(HDC hdc) { Polygon(hdc, star, 10); }
};

#endif // !OBJECT_H_


