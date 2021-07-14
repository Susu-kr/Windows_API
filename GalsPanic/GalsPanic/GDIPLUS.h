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

	//// image
	Image img1((WCHAR*)L"images/수지1.bmp");
	Image img2((WCHAR*)L"images/수지2.bmp");
	int w = img1.GetWidth();
	int h = img1.GetHeight();
	graphics.DrawImage(&img2, 0, 0, w, h);
	// Create a path that consists of a single polygon.
	Point polyPoints[] = { Point(150, 450), Point(450, 550),
	   Point(550, 550), Point(550, 450) };
	GraphicsPath path;
	path.AddPolygon(polyPoints, 4);
	// Construct a region based on the path.
	Region region(&path);
	// Draw the outline of the region.
	Pen pen(Color(255, 0, 0, 0));
	graphics.DrawPath(&pen, &path);
	// Set the clipping region of the Graphics object.
	graphics.SetClip(&region);
	graphics.DrawImage(&img1, 0, 0, w, h);

	graphics.DrawImage(&img2, 0, 0, w, h);
	Point polyPoints2[] = { Point(150, 150), Point(150, 250),
   Point(250, 250), Point(250, 150) };
	path.AddPolygon(polyPoints2, 4);
	graphics.DrawPath(&pen, &path);
	graphics.SetClip(&region);
	graphics.DrawImage(&img1, 0, 0, w, h);

}
// <<