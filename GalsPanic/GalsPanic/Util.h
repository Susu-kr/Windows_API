#pragma once

#include "framework.h"
#include <vector>

using std::vector;
using namespace Gdiplus;

// *********
//
// 여기 있는 모든 함수들은 모든 직선이 x축에 평행하거나 y축에 평행하다는 가정하에 
// 구현되었습니다.
//
// *********

// Point 객체 연산자 오버로딩
Point operator+(Point& p1, Point& p2);
Point operator-(Point& p1, Point& p2);
Point operator/(Point& p1, const int d);
bool operator==(Point& p1, Point& p2);
bool operator!=(Point& p1, Point& p2);

Point operator*(Point& p, int m);
Point operator*(int m, Point& p);

// counter clock wise 판별을 위한 함수
// 각 점들이 이루는 평행사변형의 넓이를 반환합니다.
// 양수이면 시계방향, 음수이면 반시계방향 입니다.
int ccw(const Point& pa, const Point& pb, const Point& pc);

// 폴리곤이 이루는 영역의 넓이를 구합니다.
double GetArea(const vector<Point> &polygon);

// 두 점 사이의 거리를 구합니다.
int Getdistance(const Point& p1, const Point& p2);

// 두 값 사이에 x가 있는지 확인합니다.
bool InRange(const int &x, const int &min, const int &max);

// 두 값 사이에 x가 있는지 확인합니다. 범위에 max는 포함하지 않습니다.
bool InRangeClosed(const int &x, const int &min, const int &max);

// 영역안에 점이 있는지 확인합니다.
bool InRect(const int &x, const int &y, const RECT &rect);
bool InRect(const Point &p, const RECT &rect);

// 선 위에 점이 있는지 확인합니다.
bool OnLine(const int &x, const int &y, const Point& cur, const Point& next);
bool OnLine(const Point &p, const Point& cur, const Point& next);

// 회로 위에 점이 있는지 확인합니다.
bool OnCircuit(const int &x, const int &y, const vector<Point> &circuit);
bool OnCircuit(const Point &p, const vector<Point> &circuit);

// 경로 위에 점이 있는지 확인합니다.
bool OnPath(const int &x, const int &y, const vector<Point> &path);
bool OnPath(const Point& p, const vector<Point> &path);

// 폴리곤 안에 점이 있는지 확인합니다.
// checkLine = true -> 가장자리 위에 있으면 안에 있는것으로 판단합니다.
// checkLine = false -> 가장자리 위에 있으면 안에 없는것으로 판단합니다.
bool InPolygon(const Point &p, const vector<Point> &polygon, bool checkLine);

// 선을 그립니다.
void DrawLine(Graphics *graphic, const Point& p1, const Point& p2);

// 폴리곤을 결합합니다.
void CombinePolygon(vector<Point> &polygon, vector<Point>& path);

// x, y 가 -1, 0, 1로 표현되는 방향벡터를 얻습니다.
Point GetDir(const Point& p);

// 0, 0 값을 반환합니다.
Point Zero();