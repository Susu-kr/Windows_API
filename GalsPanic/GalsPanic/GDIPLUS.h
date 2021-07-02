// >> : GDI+
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;
// << 

ULONG_PTR			g_GdiToken;

void				Gdi_Init();
void				Gdi_Draw(HDC hdc);
void				Gdi_End();
void				OnGdi_Paint(HDC hdc);

// >> GDI+
void Gdi_Init()
{
	GdiplusStartupInput gpsi;
	GdiplusStartup(&g_GdiToken, &gpsi, NULL);
}

void Gdi_Draw(HDC hdc)
{
	OnGdi_Paint(hdc);
}

void Gdi_End()
{
	GdiplusShutdown(g_GdiToken);
}

void OnGdi_Paint(HDC hdc)
{
	Graphics graphics(hdc);
	//// text print
	SolidBrush brush(Color(255, 255, 0, 0));
	//FontFamily fontFamily(L"Times New Roman");
	//Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
	//PointF pointF(10.0f, 20.0f);
	//graphics.DrawString(L"Hello GDI+", -1, &font, pointF, &brush);

	//// line
	//Pen pen(Color(255, 0, 255, 255)); // 불투명도, R, G, B
	//graphics.DrawLine(&pen, 0, 0, 200, 100);

	//// image
	Image img((WCHAR*)L"images/수지.png");
	int w = img.GetWidth();
	int h = img.GetHeight();
	//graphics.DrawImage(&img, 100, 100, w, h);

	//Image* pImg = nullptr;
	//pImg = Image::FromFile(((WCHAR*)L"images/sigong.png"));
	//int w = pImg->GetWidth();
	//int h = pImg->GetHeight();
	//Gdiplus::Matrix mat;
	//static int rot = 0;
	int xPos = 200;
	int yPos = 100;
	//mat.RotateAt(rot % 360, Gdiplus::PointF(xPos + (float)w / 2, yPos + (float)h / 2));
	//graphics.SetTransform(&mat);
	//graphics.DrawImage(pImg, 200, 100, w, h);

	//mat.RotateAt(-(rot % 360), Gdiplus::PointF(xPos + (float)w / 2, yPos + (float)h / 2));
	//graphics.SetTransform(&mat);
	//rot += 100;

	// >> : 배경 제거
	ImageAttributes imgAttr;
	imgAttr.SetColorKey(Color(240, 0, 240), Color(255, 10, 255));
	xPos = 300;
	graphics.DrawImage(&img, Rect(xPos, yPos, w, h),
		0, 0, w, h, UnitPixel, &imgAttr);
	// <<

	// >> :
	brush.SetColor(Color(128, 255, 0, 0));
	graphics.FillRectangle(&brush, 400, 300, 200, 300);
	// <<

	// >> : 투명화
	static REAL transparency = 0.5f;
	transparency += 0.1f;
	if (transparency > 1.0f) transparency = 0.0f;
	ColorMatrix colorMatrix =
	{
		//	  R		G	  B     A
			1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, transparency, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	imgAttr.SetColorMatrix(&colorMatrix);
	xPos = 400;
	graphics.DrawImage(&img, Rect(xPos, yPos, w, h),
		0, 0, w, h, UnitPixel, &imgAttr);

	colorMatrix =
	{
		//	  R		G	  B     A
			0.3f, 0.3f, 0.3f, 0.0f, 0.0f,
			0.6f, 0.6f, 0.6f, 0.0f, 0.0f,
			0.1f, 0.1f, 0.1f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};
	imgAttr.SetColorMatrix(&colorMatrix);
	xPos = 500;
	graphics.DrawImage(&img, Rect(xPos, yPos, w, h),
		0, 0, w, h, UnitPixel, &imgAttr);
	// <<

	//if (pImg) delete pImg;
}
// <<