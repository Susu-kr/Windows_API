#include "Object.h"
#include <time.h>


PLAY::PLAY(RECT & R) : CObject(R.right / 2, R.bottom - 50)
{
	degree = 90 * M_PI / 180;
	size = 50;
	ShotPos.x = center.x + (cos(degree) * (size + 50));
	ShotPos.y = center.y - (sin(degree) * (size + 50));
}

void PLAY::Update()
{
	ShotPos.x = center.x + (cos(degree) * (size + 50));
	ShotPos.y = center.y - (sin(degree) * (size + 50));
}

void PLAY::Collision(RECT & r)
{
}

void PLAY::Collision(CObject * a)
{
}

void PLAY::Draw(HDC hdc)
{
	MoveToEx(hdc, center.x, center.y, NULL);
	LineTo(hdc, ShotPos.x, ShotPos.y);
	Ellipse(hdc, center.x - size, center.y - size, center.x + size, center.y + size);
}

CCircle::CCircle(float x, float y, float d) : CObject(x, y)
{
	size = 10;
	direction.x = (cos(d) * size / 2);
	direction.y = -1 * (sin(d) * size / 2);
	type = CIRCLE;
	Active = true;
}
void CCircle::Update() {
	center.x += direction.x;
	center.y += direction.y;
}
void CCircle::Collision(RECT & r) {
	if (size > center.x || r.right - size < center.x || size > center.y)
		SetActive(false);
}

void CCircle::Collision(CObject* a)
{
	distance = (float)sqrt(pow(a->GetCenter().x - center.x, 2) + pow(a->GetCenter().y - center.y, 2));
	if (distance <= a->GetSize() + size) {
		switch (a->GetType()) {
		case CIRCLE:
			break;
		case STAR:
		{
			SetActive(false);
			a->SetActive(false);
		}
		break;
		}
	}
}


CRect::CRect(float x, float y) : CObject(x, y)
{
	size = 100;
	type = RECTANGLE;
}

void CRect::Update()
{

}

void CRect::Collision(RECT & r)
{
}

void CRect::Collision(CObject * a)
{
	distance = (float)sqrt(pow(a->GetCenter().x - center.x, 2) + pow(a->GetCenter().y - center.y, 2));
	if (distance <= a->GetSize() + size) {
		switch (a->GetType()) {
		case STAR:
		{
			SetActive(false);
		}
		break;
		}
	}
}


CStar::CStar(float x, float y) : CObject(x, y)
{
	srand((unsigned int)time(NULL));
	direction.y = rand() % 11;
	size = 50;
	degree = 2 * M_PI / 5;
	while (1) {
		int num = rand() % 20 + 1;
		if (num % 5 == 0) {
			spin = num;
			break;
		}
	}
	rotate = spin;
	type = STAR;
	Active = true;
	SetStar(rotate);

}

Pos CStar::intersection(const Pos * p1, const Pos * p2, const Pos * p3, const Pos * p4)
{
	Pos ret;
	ret.x = ((p1->x*p2->y - p1->y*p2->x)*(p3->x - p4->x) - (p1->x - p2->x)*(p3->x*p4->y - p3->y*p4->x))
		/ ((p1->x - p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x - p4->x));

	ret.y = ((p1->x*p2->y - p1->y*p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x*p4->y - p3->y*p4->x))
		/ ((p1->x - p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x - p4->x));

	return ret;
}

void CStar::SetStar(float rotate)
{
	for (int i = 0; i < 5; i++) {
		point[i].x = center.x + (cos(rotate) * (size));
		point[i].y = center.y + (sin(rotate) * (size));
		rotate += degree;
	}

	dup_p[0] = intersection(&point[0], &point[2], &point[1], &point[4]); 	// 1.5 = 1 - 3 2 - 5
	dup_p[1] = intersection(&point[0], &point[2], &point[1], &point[3]);	// 2.5 = 1 - 3 2 - 4
	dup_p[2] = intersection(&point[1], &point[3], &point[2], &point[4]);    // 3.5 = 2 - 4 3 - 5
	dup_p[3] = intersection(&point[0], &point[3], &point[2], &point[4]);	// 4.5 = 1 - 4 3 - 5
	dup_p[4] = intersection(&point[0], &point[3], &point[1], &point[4]);    // 5.5 = 1 - 4 2 - 5

	for (int i = 0; i < 5; i++) {
		star[i * 2].x = point[i].x;
		star[i * 2].y = point[i].y;
		star[i * 2 + 1].x = dup_p[i].x;
		star[i * 2 + 1].y = dup_p[i].y;
	}
}

void CStar::Update()
{
	center.y += direction.y;
	rotate += spin;
	SetStar(rotate);
}

void CStar::Collision(RECT & r)
{
	if (size > center.y) {
		center.y = size;
		direction.y *= -1;
	}
	if (r.bottom - size < center.y) {
		center.y = r.bottom - size;
		direction.y *= -1;
	}
}

void CStar::Collision(CObject * a)
{
	distance = (float)sqrt(pow(a->GetCenter().x - center.x, 2) + pow(a->GetCenter().y - center.y, 2));
	if (distance <= a->GetSize() + size) {
		switch (a->GetType()) {
		case CIRCLE:
		{
			SetActive(false);
			a->SetActive(false);
		}
		break;
		case RECTANGLE:
		{
			SetActive(false);
			a->SetActive(false);
		}
		break;
		}
	}
}
