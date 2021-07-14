#include "Util.h"
#include <algorithm>

Point operator+(Point & p1, Point & p2)
{
	return { p1.X + p2.X, p1.Y + p2.Y };
}

Point operator-(Point & p1, Point & p2)
{
	return { p1.X - p2.X, p1.Y - p2.Y };
}

Point operator/(Point & p1, const int d)
{
	Point ret = p1;

	if (d != 0)
	{
		ret.X /= d;
		ret.Y /= d;
	}

	return ret;
}

bool operator==(Point & p1, Point & p2)
{
	return (p1.X == p2.X && p1.Y == p2.Y);
}

bool operator!=(Point & p1, Point & p2)
{
	return !(p1.X == p2.X && p1.Y == p2.Y);
}

Point operator*(Point & p, int m)
{
	Point ret = p;

	ret.X *= m;
	ret.Y *= m;

	return ret;
}

Point operator*(int m, Point & p)
{
	Point ret = p;

	ret.X *= m;
	ret.Y *= m;

	return ret;
}

//   -->    -->
// | AB  X  AC |
// 을 반환합니다.

int ccw(const Point & pa, const Point & pb, const Point & pc)
{
	return (pa.X * pb.Y + pb.X * pc.Y + pc.X * pa.Y) - (pb.X * pa.Y + pa.X * pc.Y + pc.X * pb.Y);
}

// 참고
// https://ip99202.github.io/posts/%EB%B0%B1%EC%A4%80-2166-%EB%8B%A4%EA%B0%81%ED%98%95%EC%9D%98-%EB%A9%B4%EC%A0%81/

double GetArea(const vector<Point>& polygon)
{
	double sum = 0;

	int size = polygon.size();

	if (size < 3) return sum;

	for (int i = 1; i < size - 1; ++i)
		sum += (double)ccw(polygon[0], polygon[i], polygon[i + 1]);

	return sum / 2;
}

int Getdistance(const Point & p1, const Point & p2)
{
	if (p1.X == p2.X)
		return (int)abs(p1.Y - p2.Y);

	return (int)abs(p1.X - p2.X);
}

bool InRange(const int & x, const int & min, const int & max)
{
	if (min > max)
		return InRange(x, max, min);

	if (x < min || x > max)
		return false;

	return true;
}

bool InRangeClosed(const int & x, const int & min, const int & max)
{
	if (min > max)
		return InRangeClosed(x, max, min);

	if (x < min || x > max - 1)
		return false;

	return true;
}

bool InRect(const int & x, const int & y, const RECT & rect)
{
	return InRange(x, rect.left, rect.right) && InRange(y, rect.top, rect.bottom);
}

bool InRect(const Point & p, const RECT & rect)
{
	return InRange(p.X, rect.left, rect.right) && InRange(p.Y, rect.top, rect.bottom);
}

bool OnLine(const int & x, const int & y, const Point & cur, const Point & next)
{
	if (cur.X == next.X)
		if (x == cur.X && InRange(y, cur.Y, next.Y))
			return true;

	if (cur.Y == next.Y)
		if (y == cur.Y && InRange(x, cur.X, next.X))
			return true;

	return (abs(x - cur.X) == abs(y - cur.Y)) && InRange(x, cur.X, next.X) && InRange(y, cur.Y, next.Y);
}

bool OnLine(const Point & p, const Point & cur, const Point & next)
{
	if (cur.X == next.X)
		if (p.X == cur.X && InRange(p.Y, cur.Y, next.Y))
			return true;

	if (cur.Y == next.Y)
		if (p.Y == cur.Y && InRange(p.X, cur.X, next.X))
			return true;

	return (abs(p.X - cur.X) == abs(p.Y - cur.Y)) && InRange(p.X, cur.X, next.X) && InRange(p.Y, cur.Y, next.Y);
}

// 회로이기 때문에 처음 점과 마지막 점도 확인합니다.
bool OnCircuit(const int & x, const int & y, const vector<Point>& circuit)
{
	int size = (int)circuit.size();

	if (size < 2) return false;

	for (int i = 0; i < size; ++i)
	{
		Point cur = circuit[i];
		Point next = circuit[(i + 1) % size];

		if (OnLine(x, y, cur, next))
			return true;
	}

	return false;
}

bool OnCircuit(const Point & p, const vector<Point>& circuit)
{
	int size = (int)circuit.size();

	if (size < 2) return false;

	for (int i = 0; i < size; ++i)
	{
		Point cur = circuit[i];
		Point next = circuit[(i + 1) % size];

		if (OnLine(p, cur, next))
			return true;
	}

	return false;
}

// 경로이기 때문에 처음점과 마지막 점은 확인하지 않습니다.
bool OnPath(const int & x, const int & y, const vector<Point>& path)
{
	int size = (int)path.size();

	if (size < 2) return false;

	for (int i = 0; i < size - 1; ++i)
	{
		Point cur = path[i];
		Point next = path[i + 1];

		if (OnLine(x, y, cur, next))
			return true;
	}

	return false;
}

bool OnPath(const Point & p, const vector<Point>& path)
{
	int size = (int)path.size();

	if (size < 2) return false;

	for (int i = 0; i < size - 1; ++i)
	{
		Point cur = path[i];
		Point next = path[i + 1];

		if (OnLine(p, cur, next))
			return true;
	}

	return false;
}

// 참고
// https://3001ssw.tistory.com/124
bool InPolygon(const Point & p, const vector<Point>& polygon, bool checkLine)
{
	int size = (int)polygon.size();

	if (size < 3)
		return false;

	// 경로 위인지 확인
	if (checkLine && OnCircuit(p, polygon))
		return true;
	else if (!checkLine && OnCircuit(p, polygon))
		return false;

	int rightCnt = 0;

	for (int i = 0; i < size; ++i)
	{
		Point cur = polygon[i];
		Point next = polygon[(i + 1) % size];

		// Y축과 평행한 선분만 확인한다
		if (cur.X == next.X)
		{
			if (InRangeClosed(p.Y, cur.Y, next.Y) && p.X < cur.X && p.X < next.X)
				++rightCnt;
		}
	}

	return rightCnt % 2 == 1;
}

void DrawLine(Graphics * graphic, const Point& p1, const Point& p2)
{
	Pen pen(Color(255, 255, 0, 255));
	graphic->DrawLine(&pen, p1, p2);
}

// 주어진 입력값으로 새로운 폴리곤을 만듭니다.
vector<Point> CreateNewPolygon(const vector<Point>& polygon, const vector<Point>& path)
{
	vector<Point> newPolygon;

	// 일단 플레이어가 지나온 경로부터 삽입
	newPolygon.insert(newPolygon.end(), path.begin(), path.end());

	int beginIdx = -1, endIdx = -1;
	int size = polygon.size();

	// 경로의 시작과 끝이 각각 폴리곤의 어느 변과 만나는지 구합니다.
	for (int i = 0; i < size; ++i)
	{
		Point cur = polygon[i];
		Point next = polygon[(i + 1) % size];

		if (beginIdx == -1 && OnLine(path.front(), cur, next))
			beginIdx = i;

		if (endIdx == -1 && OnLine(path.back(), cur, next))
			endIdx = i;
	}

	// 경로의 시작과 끝이 같은 변 위에 있을 때 처리
	if (beginIdx == endIdx)
	{
		int next = (beginIdx + 1) % size;

		// front to next : 경로의 시작점에서 폴리곤 위 다음 점까지의 거리
		int fToN = Getdistance(path.front(), polygon[next]);

		// back to next : 경로의 끝점에서 폴리곤 위 다음 점까지의 거리
		int bToN = Getdistance(path.back(), polygon[next]);

		// 끝점이 시작점보다 뒤에 있을 때
		if (bToN > fToN)
			return newPolygon;
	}

	// 경로의 시작부터 끝까지 순회하며 새로운 폴리곤을 구성합니다.
	int i = endIdx + 1;

	while (true)
	{
		Point cur = polygon[i % size];
		Point next = polygon[(i + 1) % size];

		newPolygon.push_back(cur);

		if (OnLine(path.front(), cur, next))
			break;

		++i;
	}

	return newPolygon;
}

void CombinePolygon(vector<Point>& polygon, vector<Point>& path)
{
	// 원래 경로와 뒤집힌 경로로부터 만들어진 폴리곤들의 넓이를 비교하여
	// 더 큰영역을 선택합니다.

	vector<Point> combined1 = CreateNewPolygon(polygon, path);

	std::reverse(path.begin(), path.end());

	vector<Point> combined2 = CreateNewPolygon(polygon, path);

	double area1 = GetArea(combined1);
	double area2 = GetArea(combined2);

	polygon = area1 > area2 ? combined1 : combined2;
	path.clear();
}

Point GetDir(const Point & p)
{
	Point ret = p;

	if (ret.X != 0) ret.X /= abs(ret.X);
	if (ret.Y != 0) ret.Y /= abs(ret.Y);

	return ret;
}

Point Zero()
{
	Point ret = { 0, 0 };
	return ret;
}