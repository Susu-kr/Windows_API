#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_
#define _USE_MATH_DEFINES
#include "framework.h"
#include "Defense_Game.h"
#include <cmath>

enum TYPE { CIRCLE, STAR, RECTANGLE, PLAYER };

struct Pos {
	float x;
	float y;
};

class CObject {
protected:
	Pos center;
	Pos direction;
	float distance;
	Pos		ShotPos;
	float degree;
	float size;
	bool Active;
	TYPE type;
public:
	CObject(float x, float y) : center({ x, y }), direction({ 0, 0 }) {}
	CObject(float x, float y, float d_x, float d_y, int s) : center({ x,y }), direction({ d_x,d_y }), size(s) {}
	Pos		GetCenter() { return center; }
	Pos		GetDirection() { return direction; }
	TYPE	GetType() { return type; }
	float	GetSize() { return size; }
	bool	GetActive() { return Active; }
	float	GetDegree() { return degree; }
	Pos		GetShotPos() { return ShotPos; }

	void	SetDegree(float d) { degree = d; }
	void	SetSize(int s) { size = s; }
	void	SetActive(bool a) { Active = a; }
	void	SetCenter(float x, float y) { center = { x, y }; }
	void	SetDirection(float x, float y) { direction = { x, y }; }
	void	SetShotPos(float x, float y) { ShotPos.x = x; ShotPos.y = y; }

	virtual void Update() = 0;					// 좌표를 업데이트
	virtual void Collision(RECT & r) = 0;		// 충돌 여부 확인
	virtual void Collision(CObject * a) = 0;	// 충돌 여부 확인
	virtual void Draw(HDC hdc) = 0;				// 그려주기
};

class PLAY : public CObject 
{
private:

public:
	PLAY(RECT & R);

	virtual void Update(); // 그려주기
	virtual void Collision(RECT & r); // 충돌 여부 확인
	virtual void Collision(CObject * a);
	virtual void Draw(HDC hdc);
};

class CCircle : public CObject
{
public:
	CCircle(float x, float y, float d);
	CCircle(float x, float y, float d_x, float d_y, int size) : CObject(x, y, d_x, d_y, size) {}
	virtual void Update(); // 그려주기
	virtual void Collision(RECT & r); // 충돌 여부 확인
	virtual void Collision(CObject * a);
	virtual void Draw(HDC hdc) { Ellipse(hdc, center.x - size, center.y - size, center.x + size, center.y + size); }

};

class CRect : public CObject
{

public:
	CRect(float x, float y);
	virtual void Update(); // 그려주기
	virtual void Collision(RECT & r); // 충돌 여부 확인
	virtual void Collision(CObject * a);
	virtual void Draw(HDC hdc) {
		Rectangle(hdc, center.x - size/2, center.y - size/4,
			center.x + size/2, center.y + size/4);
	}
};

class CStar : public CObject
{
private:
	float spin;
	float rotate;
	float degree;
	POINT star[10];
	Pos point[5];
	Pos dup_p[5];
public:
	CStar(float x, float y);
	CStar(float x, float y, float d_x, float d_y, int size) : CObject(x, y, d_x, d_y, size) {}

	Pos intersection(const Pos* p1, const Pos* p2, const Pos* p3, const Pos* p4);
	void SetStar(float spin);

	virtual void Update(); // 그려주기
	virtual void Collision(RECT & r); // 충돌 여부 확인
	virtual void Collision(CObject * a);
	virtual void Draw(HDC hdc) { Polygon(hdc, star, 10); }

};

#endif // !OBJECT_H_


