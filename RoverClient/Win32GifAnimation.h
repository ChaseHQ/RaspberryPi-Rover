#include <ObjIdl.h>
#include <Windows.h>
#include <algorithm>
namespace Gdiplus
{
  using std::min;
  using std::max;
}
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

#include "resource.h"
#include "Win32CustomControl.h"

#ifndef __WIN32GIFANIMATION__
#define __WIN32GIFANIMATION__

using namespace Gdiplus;

class Win32GifAnimation : public Win32CustomControl {
public:
	Win32GifAnimation();
	~Win32GifAnimation();
	void SetImageFromResourceName(const WORD wResource);
	void StartAnimating(UINT elapse);
	void StopAnimating();
protected:
	bool onPaint(const HWND& hWnd, const PAINTSTRUCT& ps);
	bool onTimer(const WPARAM& wParam);
private:
	ULONG_PTR __gdiplusToken;
	Image * __image;
	WORD __numFrames;
	WORD __currentFrame;
};

#endif