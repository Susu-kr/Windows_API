#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_
#define _USE_MATH_DEFINES
#include "framework.h"
#include "Chapter4.h"
#include <cmath>

/*
	1. 해당 위치에 원을 생성
	2. 임의 방향으로 이동 (timer, random) 클라이언트 외곽에서 반사 (입사각, 반사각)
	3. 다른 오브젝트와 충돌시
		3.1 기본 서로 튕기도록 처리
		3.2 합체 - 상성관계에 따라 한 타입의 도형의 크기 증가, 다른 타입 도형 삭제
		3.3 분열 - 상성관계에 따라 작은 크기의 도형으로 분열 하도록
			별 > 원 > 사각형 > 별 .. 같은 상성끼리는 튕기도록.
		3.4 일정 크기 이상은 그냥 사라지고, 일정 크기 이하도 그냥 사라지도록 한다.
	4. 랜덤으로 사각형, 원, 별이 그러지도록
	5. 각 오브젝트를 회전 시킨다.
*/

enum TYPE { CIRCLE, STAR, RECTANGLE };
enum MODE { NORMAL, BIGGER, SMALLER };

struct Pos {
	float x;
	float y;
};

class CObject {
protected:
	Pos center;
	Pos direction;
	float distance;
	float size;
	bool Active;
	TYPE type;
public:
	CObject(float x, float y) : center({ x, y }), direction({ 0, 0 }) {}
	CObject(float x, float y, float d_x, float d_y, int s) : center({ x,y }), direction({ d_x,d_y }), size(s) {}
	Pos		GetCenter()			{ return center; }
	Pos		GetDirection()		{ return direction; }
	TYPE	GetType()			{ return type; }
	float	GetSize()			{ return size; }
	bool	GetActive()			{ return Active; }

	void	SetSize(int s)					{ size = s; }
	void	SetActive(bool a)				{ Active = a; }
	void	SetCenter(float x, float y)		{ center = { x, y }; }
	void	SetDirection(float x, float y)	{ direction = { x, y }; }
	void	BOUNCE(CObject * a);
	void	OVERLAPS(CObject * a);
	void	COMBINES();
	void	SPLITS();

	virtual void Update() = 0;					// 좌표를 업데이트
	virtual void Collision(RECT & r) = 0;		// 충돌 여부 확인
	virtual void Collision(CObject * a, MODE mode) = 0;	// 충돌 여부 확인
	virtual void Draw(HDC hdc) = 0;				// 그려주기
};


class CCircle : public CObject
{
public:
	CCircle(float x, float y);
	CCircle(float x, float y, float d_x, float d_y, int size) : CObject(x, y, d_x, d_y, size) {}
	virtual void Update(); // 그려주기
	virtual void Collision(RECT & r); // 충돌 여부 확인
	virtual void Collision(CObject * a, MODE mode);
	virtual void Draw(HDC hdc) { Ellipse(hdc, center.x - size, center.y - size, center.x + size, center.y + size); }

};

class CRect : public CObject
{
private:
	POINT Rect[4];
	float spin;
	float degree;
	float rotate;
public:
	CRect(float x, float y);
	CRect(float x, float y, float d_x, float d_y, int size) : CObject(x, y, d_x, d_y, size) {}

	void	 SetRect();

	virtual void Update(); // 그려주기
	virtual void Collision(RECT & r); // 충돌 여부 확인
	virtual void Collision(CObject * a, MODE mode);
	virtual void Draw(HDC hdc) { SetRect(); Polygon(hdc, Rect, 4); }

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
	virtual void Collision(CObject * a, MODE mode);
	virtual void Draw(HDC hdc) { Polygon(hdc, star, 10); }

};

#endif // !OBJECT_H_


