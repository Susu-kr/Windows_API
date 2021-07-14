#pragma once

#include "framework.h"
#include <vector>

using std::vector;
using namespace Gdiplus;

// *********
//
// ���� �ִ� ��� �Լ����� ��� ������ x�࿡ �����ϰų� y�࿡ �����ϴٴ� �����Ͽ� 
// �����Ǿ����ϴ�.
//
// *********

// Point ��ü ������ �����ε�
Point operator+(Point& p1, Point& p2);
Point operator-(Point& p1, Point& p2);
Point operator/(Point& p1, const int d);
bool operator==(Point& p1, Point& p2);
bool operator!=(Point& p1, Point& p2);

Point operator*(Point& p, int m);
Point operator*(int m, Point& p);

// counter clock wise �Ǻ��� ���� �Լ�
// �� ������ �̷�� ����纯���� ���̸� ��ȯ�մϴ�.
// ����̸� �ð����, �����̸� �ݽð���� �Դϴ�.
int ccw(const Point& pa, const Point& pb, const Point& pc);

// �������� �̷�� ������ ���̸� ���մϴ�.
double GetArea(const vector<Point> &polygon);

// �� �� ������ �Ÿ��� ���մϴ�.
int Getdistance(const Point& p1, const Point& p2);

// �� �� ���̿� x�� �ִ��� Ȯ���մϴ�.
bool InRange(const int &x, const int &min, const int &max);

// �� �� ���̿� x�� �ִ��� Ȯ���մϴ�. ������ max�� �������� �ʽ��ϴ�.
bool InRangeClosed(const int &x, const int &min, const int &max);

// �����ȿ� ���� �ִ��� Ȯ���մϴ�.
bool InRect(const int &x, const int &y, const RECT &rect);
bool InRect(const Point &p, const RECT &rect);

// �� ���� ���� �ִ��� Ȯ���մϴ�.
bool OnLine(const int &x, const int &y, const Point& cur, const Point& next);
bool OnLine(const Point &p, const Point& cur, const Point& next);

// ȸ�� ���� ���� �ִ��� Ȯ���մϴ�.
bool OnCircuit(const int &x, const int &y, const vector<Point> &circuit);
bool OnCircuit(const Point &p, const vector<Point> &circuit);

// ��� ���� ���� �ִ��� Ȯ���մϴ�.
bool OnPath(const int &x, const int &y, const vector<Point> &path);
bool OnPath(const Point& p, const vector<Point> &path);

// ������ �ȿ� ���� �ִ��� Ȯ���մϴ�.
// checkLine = true -> �����ڸ� ���� ������ �ȿ� �ִ°����� �Ǵ��մϴ�.
// checkLine = false -> �����ڸ� ���� ������ �ȿ� ���°����� �Ǵ��մϴ�.
bool InPolygon(const Point &p, const vector<Point> &polygon, bool checkLine);

// ���� �׸��ϴ�.
void DrawLine(Graphics *graphic, const Point& p1, const Point& p2);

// �������� �����մϴ�.
void CombinePolygon(vector<Point> &polygon, vector<Point>& path);

// x, y �� -1, 0, 1�� ǥ���Ǵ� ���⺤�͸� ����ϴ�.
Point GetDir(const Point& p);

// 0, 0 ���� ��ȯ�մϴ�.
Point Zero();