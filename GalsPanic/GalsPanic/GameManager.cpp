#include "GameManager.h"
#include "Util.h"

GameManager::GameManager()
	:input(new InputManager())
{ }

GameManager::~GameManager()
{
	delete input;
}

void GameManager::Init()
{
	// ȭ���� ����, ���� ũ�⸦ �����ɴϴ�.
	screenW = GetSystemMetrics(SM_CXSCREEN);
	screenH = GetSystemMetrics(SM_CYSCREEN);

	// ��� �̹��� ������ �ҷ��ɴϴ�.
	image = new Image(L"images/����1.bmp");

	// �̹��� ������ ����, ���� ũ�⸦ �����ɴϴ�.
	imageW = image->GetWidth();
	imageH = image->GetHeight();

	// �̹��� ������ ������ �����մϴ�.
	RECT rect = { 0, 0, imageW, imageH };

	// �÷��̾ �����մϴ�, �ʱ� ��ġ�� �����մϴ�.
	player = new Player(200, 200, rect);

	// �ʱ� ���� ������ �����մϴ�.
	// �ð�������� �����մϴ�.
	opened.push_back({ 200, 200 });
	opened.push_back({ 300, 200 });
	opened.push_back({ 300, 300 });
	opened.push_back({ 200, 300 });
}

void GameManager::Update()
{
	// ������Ʈ �ֱ⸦ �����մϴ�.

	DWORD newTime = GetTickCount();
	static DWORD oldTime = newTime;

	if (newTime - oldTime < 16)
		return;

	oldTime = newTime;

	// �÷��̾��� �Է°��� �����ɴϴ�.
	int dirX = input->GetHorizontal();
	int dirY = input->GetVertical();
	bool space = input->GetKeySpace();


	if (space)	// ���� ������ ��θ� �׸��鼭 �������ϴ�.
		player->MoveWithSpace(dirX, dirY, opened);
	else if (dirX == 0 && dirY == 0)	// ��θ� ���� �ǵ��� ���ϴ�.
		player->MoveBack();
	else // ���� ���� ��������� �̵��մϴ�.
		player->MoveWithoutSpace(dirX, dirY, opened);
}

// ���� ������ �׸��ϴ�.
void GameManager::DrawClosed(Graphics * graphic)
{
	image = new Image(L"images/����2.bmp");

	// �̹��� ������ ����, ���� ũ�⸦ �����ɴϴ�.
	imageW = image->GetWidth();
	imageH = image->GetHeight();
	SolidBrush brush(Color(0, 0, 0, 0));
	graphic->FillRectangle(&brush, 0, 0, imageW, imageH);
}

// ���� ������ �׸��ϴ�. ������ ������� Ŭ���� �մϴ�.
void GameManager::DrawOpened(Graphics * graphic)
{
	Point *points = new Point[opened.size()];

	for (int i = 0; i < opened.size(); ++i)
		points[i] = opened[i];

	GraphicsPath path;

	path.AddPolygon(points, (int)opened.size());
	Region region(&path);

	graphic->SetClip(&region);

	graphic->DrawImage(image, 0, 0, imageW, imageH);

	Pen pen(Color(255, 0, 255, 255));
	graphic->DrawPath(&pen, &path);

	graphic->ResetClip();

	delete[] points;
}

// ���� �÷��̸� �׸��ϴ�.
void GameManager::DrawGame(Graphics *graphic)
{
	DrawClosed(graphic);
	DrawOpened(graphic);
	player->DrawPlayer(graphic);
}

// ������ü�� �׸��ϴ�. ���� ���۸��� ����մϴ�.
void GameManager::Draw(HWND hWnd, HDC hdc)
{
	HDC memDC;
	HBITMAP oldBit, newBit;

	memDC = CreateCompatibleDC(hdc);

	newBit = CreateCompatibleBitmap(hdc, imageW, imageH);
	oldBit = (HBITMAP)SelectObject(memDC, newBit);

	Graphics *graphic = new Graphics(memDC);

	// �ȼ� ����� ����
	graphic->SetCompositingQuality(CompositingQuality::CompositingQualityAssumeLinear);
	graphic->SetSmoothingMode(SmoothingMode::SmoothingModeAntiAlias);
	graphic->SetInterpolationMode(InterpolationMode::InterpolationModeHighQualityBicubic);
	graphic->SetPixelOffsetMode(PixelOffsetMode::PixelOffsetModeHighQuality);

	DrawGame(graphic);

	delete graphic;

	BitBlt(hdc, 0, 0, imageW, imageH, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, oldBit);
	DeleteObject(newBit);

	DeleteDC(memDC);
}