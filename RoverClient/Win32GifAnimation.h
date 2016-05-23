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

#include "Win32CustomControl.h"

#ifndef __WIN32GIFANIMATION__
#define __WIN32GIFANIMATION__

using namespace Gdiplus;

class Win32GifAnimation : public Win32CustomControl {
public:
	Win32GifAnimation();
	~Win32GifAnimation();
protected:
	void onPaint(const HWND& hWnd, const PAINTSTRUCT& ps);
private:
	ULONG_PTR __gdiplusToken;
};

#endif