#pragma once
#ifndef OBJECT_H_
#define OBJECT_H_
#define _USE_MATH_DEFINES
#include "framework.h"
#include "Chapter4.h"
#include <cmath>

/*
	1. �ش� ��ġ�� ���� ����
	2. ���� �������� �̵� (timer, random) Ŭ���̾�Ʈ �ܰ����� �ݻ� (�Ի簢, �ݻ簢)
	3. �ٸ� ������Ʈ�� �浹��
		3.1 �⺻ ���� ƨ�⵵�� ó��
		3.2 ��ü - �󼺰��迡 ���� �� Ÿ���� ������ ũ�� ����, �ٸ� Ÿ�� ���� ����
		3.3 �п� - �󼺰��迡 ���� ���� ũ���� �������� �п� �ϵ���
			�� > �� > �簢�� > �� .. ���� �󼺳����� ƨ�⵵��.
		3.4 ���� ũ�� �̻��� �׳� �������, ���� ũ�� ���ϵ� �׳� ��������� �Ѵ�.
	4. �������� �簢��, ��, ���� �׷�������
	5. �� ������Ʈ�� ȸ�� ��Ų��.
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

	virtual void Update() = 0;					// ��ǥ�� ������Ʈ
	virtual void Collision(RECT & r) = 0;		// �浹 ���� Ȯ��
	virtual void Collision(CObject * a, MODE mode) = 0;	// �浹 ���� Ȯ��
	virtual void Draw(HDC hdc) = 0;				// �׷��ֱ�
};


class CCircle : public CObject
{
public:
	CCircle(float x, float y);
	CCircle(float x, float y, float d_x, float d_y, int size) : CObject(x, y, d_x, d_y, size) {}
	virtual void Update(); // �׷��ֱ�
	virtual void Collision(RECT & r); // �浹 ���� Ȯ��
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

	virtual void Update(); // �׷��ֱ�
	virtual void Collision(RECT & r); // �浹 ���� Ȯ��
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

	virtual void Update(); // �׷��ֱ�
	virtual void Collision(RECT & r); // �浹 ���� Ȯ��
	virtual void Collision(CObject * a, MODE mode);
	virtual void Draw(HDC hdc) { Polygon(hdc, star, 10); }

};

#endif // !OBJECT_H_


