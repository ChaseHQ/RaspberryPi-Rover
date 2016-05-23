#include "Win32GifAnimation.h"

Win32GifAnimation::Win32GifAnimation() : __currentFrame(0), __numFrames(0), __image(nullptr), Win32CustomControl("Win32GifAnimation") {
	GdiplusStartupInput si;
	GdiplusStartup(&__gdiplusToken,&si,NULL);
}

Win32GifAnimation::~Win32GifAnimation() {
	if (__image)
		delete __image;

	GdiplusShutdown(__gdiplusToken);
}

bool Win32GifAnimation::onPaint(const HWND& hWnd, const PAINTSTRUCT& ps) {
	//FrameRect(ps.hdc,&_getBounds(),(HBRUSH)GetStockObject(BLACK_BRUSH));

	Graphics graphics(ps.hdc);
	GUID dId[1];
	RECT r = _getBounds();
	
	__image->GetFrameDimensionsList(dId,1);
	__image->SelectActiveFrame(dId, __currentFrame);
	graphics.DrawImage(__image,0,0, r.right, r.bottom);

	return true;
}

void Win32GifAnimation::SetImageFromResourceName(const WORD wResource) {
	IStream * stream = nullptr;

	HINSTANCE hInst = GetModuleHandle(0);
	HRSRC hC = FindResource(hInst, MAKEINTRESOURCE(wResource), "RC_DATA");
	HGLOBAL hG = LoadResource(hInst, hC);
	void * bytes = LockResource(hG);
	ULONG size = SizeofResource(hInst, hC);
	HRESULT hr = CreateStreamOnHGlobal(NULL, true,&stream);
	ULONG written;
	hr = stream->Write(bytes, size, &written);
	
	if (__image) delete __image;
	__image = new Image(stream);
	
	GUID dId[1];
	__image->GetFrameDimensionsList(dId, 1);
	__numFrames = __image->GetFrameCount(dId);

}

void Win32GifAnimation::StartAnimating(UINT elapse) {
	UINT ptr = SetTimer(_getWindow(), 100, elapse, NULL);
}

void Win32GifAnimation::StopAnimating() {
	KillTimer(_getWindow(), 100);
}

bool Win32GifAnimation::onTimer(const WPARAM& wParam) {
	InvalidateRect(_getWindow(), &_getBounds(), true);
	if (++__currentFrame == (__numFrames - 1)) __currentFrame = 0;
	return true;
}