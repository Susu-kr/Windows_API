#include "Object.h"
#include <time.h>



void PLAY::Draw(HDC hdc)
{
	MoveToEx(hdc, center.x, center.y, NULL);
	LineTo(hdc, ShotPos.x, ShotPos.y);
	Ellipse(hdc, center.x - size, center.y - size, center.x + size, center.y + size);
}

Bullet::Bullet(float x, float y, float d) : STRUCT(x, y)
{
	size = 10;
	direction.x = (cos(d) * size / 2);
	direction.y = -1 * (sin(d) * size / 2);
	type = CIRCLE;
	Active = true;
}
void Bullet::Update() {
	center.x += direction.x;
	center.y += direction.y;
}
void Bullet::Collision(RECT & r) {
	if (size > center.x || r.right - size < center.x || size > center.y)
		SetActive(false);
}

void Bullet::Collision(STRUCT* a)
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


Wall::Wall(float x, float y) : STRUCT(x, y)
{
	size = 100;
	type = RECTANGLE;
}

void Wall::Update()
{
	
}

void Wall::Collision(RECT & r)
{
}

void Wall::Collision(STRUCT * a)
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


Enemy::Enemy(float x, float y) : STRUCT(x, y)
{
	srand((unsigned int)time(NULL));
	degree = 2 * M_PI / 5;
	while (1) {
		int num = rand() % 20 + 1;
		if (num % 5 == 0) {
			spin = num;
			break;
		}
	}
	rotate = spin;
	Active = true;
	SetStar(rotate);

}

Pos Enemy::intersection(const Pos * p1, const Pos * p2, const Pos * p3, const Pos * p4)
{
	Pos ret;
	ret.x = ((p1->x*p2->y - p1->y*p2->x)*(p3->x - p4->x) - (p1->x - p2->x)*(p3->x*p4->y - p3->y*p4->x))
		/ ((p1->x - p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x - p4->x));

	ret.y = ((p1->x*p2->y - p1->y*p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x*p4->y - p3->y*p4->x))
		/ ((p1->x - p2->x)*(p3->y - p4->y) - (p1->y - p2->y)*(p3->x - p4->x));

	return ret;
}

void Enemy::SetStar(float rotate)
{
	for (int i = 0; i < 5; i++) {
		point[i].x = center.x + (cos(rotate) * (scale));
		point[i].y = center.y + (sin(rotate) * (scale));
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

void Enemy::Update()
{
	center.y += direction.y;
	rotate += spin;
	SetStar(rotate);
}

void Enemy::Collision(RECT & r)
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

void Enemy::Collision(STRUCT * a)
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
