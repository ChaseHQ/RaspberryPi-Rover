#include "Win32GifAnimation.h"

Win32GifAnimation::Win32GifAnimation() : Win32CustomControl("Win32GifAnimation") {
	GdiplusStartupInput si;
	GdiplusStartup(&__gdiplusToken,&si,NULL);
}

Win32GifAnimation::~Win32GifAnimation() {
	GdiplusShutdown(__gdiplusToken);
}

void Win32GifAnimation::onPaint(const HWND& hWnd, const PAINTSTRUCT& ps) {
	FrameRect(ps.hdc,&_getBounds(),(HBRUSH)GetStockObject(BLACK_BRUSH));
	
}