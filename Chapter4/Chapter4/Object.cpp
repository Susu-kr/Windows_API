#include "Object.h"
#include <time.h>

CCircle::CCircle(float x, float y) : CObject(x, y)
{
	srand((unsigned int)time(NULL));
	direction.x = rand() % 11 - 5.0f * 2.0f;
	direction.y = rand() % 11 - 5.0f * 2.0f;
	size = 50;
	type = CIRCLE;
	Active = true;
}
void CCircle::Update() {
	center.x += direction.x;
	center.y += direction.y;
}
void CCircle::Collision(RECT & r) {
	if (size > center.x) {
		center.x = size;
		direction.x *= -1;
	}
	if (r.right - size < center.x) {
		center.x = r.right - size;
		direction.x *= -1;
	}
	if (size > center.y) {
		center.y = size;
		direction.y *= -1;
	}
	if (r.bottom - size < center.y) {
		center.y = r.bottom - size;
		direction.y *= -1;
	}
}

void CCircle::Collision(CObject* a, MODE mode)
{
	// 원 -> 별 :  별에게 합체 OR 별이 원으로 분열
	// 사각형 -> 원 : 원에게 사각형 합체 OR 원이 사각형으로 분열
	distance = (float)sqrt(pow(a->GetCenter().x - center.x, 2) + pow(a->GetCenter().y - center.y, 2));
	if (distance <= a->GetSize() + size) {
		switch (a->GetType()) {
		case CIRCLE:
			BOUNCE(a);
			break;
		case RECTANGLE:
		{
			switch (mode) {
			case NORMAL:
				BOUNCE(a);
				break;
			case BIGGER: {
				COMBINES();
				a->SetActive(false);
			}
				break;
			case SMALLER: {
				SPLITS();
				a->SetActive(false); 
			}
				break;
			}
		}
		break;
		case STAR:
		{
			switch (mode) {
			case NORMAL:
				BOUNCE(a);
				break;
			case BIGGER: {
				SetActive(false);
				a->COMBINES();
			}
				break;
			case SMALLER: {
				SetActive(false);
				a->SPLITS();
			}
				break;
			}
		}
		break;
		}
	}
}

CRect::CRect(float x, float y) : CObject(x, y)
{
	srand((unsigned int)time(NULL));
	direction.x = rand() % 11 - 5.0f * 2.0f;
	direction.y = rand() % 11 - 5.0f * 2.0f;
	size = 50;
	spin = rand() % 5 + 5;
	rotate = spin;
	type = RECTANGLE;
	Active = true;
	SetRect();

}

void CRect::SetRect()
{
	for (int i = 0; i < 4; i++) {
		int deg = (90 * i + (int)rotate) % 360;
		degree = deg * M_PI / 180.0;
		Rect[i].x = center.x + (cos(degree) * (size));
		Rect[i].y = center.y + (sin(degree) * (size));
	}
}

void CRect::Update()
{
	center.x += direction.x;
	center.y += direction.y;
	rotate += spin;
}

void CRect::Collision(RECT & r)
{
	if (size > center.x) {
		center.x = size;
		direction.x *= -1;
	}
	if (r.right - size < center.x) {
		center.x = r.right - size;
		direction.x *= -1;
	}
	if (size > center.y) {
		center.y = size;
		direction.y *= -1;
	}
	if (r.bottom - size < center.y) {
		center.y = r.bottom - size;
		direction.y *= -1;
	}
}

void CRect::Collision(CObject * a, MODE mode)
{
	distance = (float)sqrt(pow(a->GetCenter().x - center.x, 2) + pow(a->GetCenter().y - center.y, 2));
	if (distance <= a->GetSize() + size) {
		switch (a->GetType()) {
		case CIRCLE:
		{
			switch (mode) {
			case NORMAL:
				BOUNCE(a);
				break;
			case BIGGER:
			{
				SetActive(false);
				a->COMBINES();
			}
			break;
			case SMALLER: {
				SetActive(false);
				a->SPLITS();
			}
				break;
			}
		}
		break;
		case RECTANGLE:
			BOUNCE(a);
			break;
		case STAR:
		{
			switch (mode) {
			case NORMAL:
				BOUNCE(a);
				break;
			case BIGGER: {
				COMBINES();
				a->SetActive(false);
			}
				break;
			case SMALLER: {
				SPLITS();
				a->SetActive(false);
			}
				break;
			}
		}
		break;
		}
	}
}


CStar::CStar(float x, float y) : CObject(x, y)
{
	srand((unsigned int)time(NULL));
	direction.x = rand() % 11 - 5.0f * 2.0f;
	direction.y = rand() % 11 - 5.0f * 2.0f;
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
	center.x += direction.x;
	center.y += direction.y;
	rotate += spin;
	SetStar(rotate);
}

void CStar::Collision(RECT & r)
{
	if (size > center.x) {
		center.x = size;
		direction.x *= -1;
	}
	if (r.right - size < center.x) {
		center.x = r.right - size;
		direction.x *= -1;
	}
	if (size > center.y) {
		center.y = size;
		direction.y *= -1;
	}
	if (r.bottom - size < center.y) {
		center.y = r.bottom - size;
		direction.y *= -1;
	}
}

void CStar::Collision(CObject * a, MODE mode)
{
	distance = (float)sqrt(pow(a->GetCenter().x - center.x, 2) + pow(a->GetCenter().y - center.y, 2));
	if (distance <= a->GetSize() + size) {
		switch (a->GetType()) {
		case CIRCLE:
		{
			switch (mode) {
			case NORMAL:
				BOUNCE(a);
				break;
			case BIGGER:
				COMBINES();
				a->SetActive(false);
				break;
			case SMALLER: {
				SPLITS();
				a->SetActive(false);
			}
				break;
			}
		}
		break;
		case RECTANGLE:
		{
			switch (mode) {
			case NORMAL:
				BOUNCE(a);
				break;
			case BIGGER:
				SetActive(false);
				a->COMBINES();
				break;
			case SMALLER: {
				SetActive(false);
				a->SPLITS();
			}
				break;
			}
		}
		break;
		case STAR:
			BOUNCE(a);
			break;
		}
	}
}

void CObject::BOUNCE(CObject * a)
{
	// 노멀 벡터 (원1 중심 - 원2 중심)의 정규화
	float Nx = (a->GetCenter().x - center.x) / distance;
	float Ny = (a->GetCenter().y - center.y) / distance;

	// 접선 벡터
	float Ax = -Ny;
	float By = Nx;

	// 내적 접선 벡터
	float dpTan1 = direction.x * Ax + direction.y * By;
	float dpTan2 = a->GetDirection().x * Ax + a->GetDirection().y * By;

	// dir 내적 노멀 벡터
	float dpNorm1 = direction.x * Nx + direction.y * Ny;
	float dpNorm2 = a->GetDirection().x * Nx + a->GetDirection().y * Ny;

	SetDirection(Ax * dpTan1 + Nx * dpNorm2, By * dpTan1 + Ny * dpNorm2);
	a->SetDirection(Ax * dpTan2 + Nx * dpNorm1, By * dpTan2 + Ny * dpNorm1);

	if (distance < a->GetSize() + GetSize()) {
		OVERLAPS(a);
	}
}

void CObject::OVERLAPS(CObject * a)
{
	if (distance < a->GetSize() + GetSize()) {
		LONG overlaps = 0.5f * (distance - a->GetSize() - GetSize());

		center.x -= overlaps * (center.x - a->GetCenter().x) / distance;
		center.y -= overlaps * (center.y - a->GetCenter().y) / distance;

		a->SetCenter(a->GetCenter().x + overlaps * (center.x - a->GetCenter().x) / distance
			, a->GetCenter().y + overlaps * (center.y - a->GetCenter().y) / distance);
	}
}

void CObject::COMBINES()
{
	size += 25;
	if (size > 200) SetActive(false);
}

void CObject::SPLITS()
{
	size -= 25;
	if (size < 10) SetActive(false);
}

