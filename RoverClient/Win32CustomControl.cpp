#include "Win32CustomControl.h"

std::atomic_uint Win32CustomControl::__CLASSCOUNT;

Win32CustomControl::Win32CustomControl(const char * className) : __hwnd(NULL), __CLASSNAME(className) {
	__mtx.lock();
	if (__CLASSCOUNT++ == 0) {
		// Initialize WNDClass 
		__initializeWndClass();
	}
	__mtx.unlock();
}

Win32CustomControl::~Win32CustomControl() {
	__mtx.lock();
	if (--__CLASSCOUNT == 0) {
		// Last Class UnInitialize
		__destroyWndClass();
	}
	__mtx.unlock();
}

void Win32CustomControl::CreateControl(const HWND &parent, const RECT &bounds) {
	__hwnd = CreateWindow(__CLASSNAME.c_str(),NULL, WS_CHILD | WS_VISIBLE, bounds.left,bounds.top,bounds.right,bounds.bottom,parent,NULL,GetModuleHandle(NULL),NULL);
	SetWindowLongPtr(__hwnd,GWLP_USERDATA,reinterpret_cast<LONG>(this));
	__hwParent = parent;
}

LRESULT CALLBACK Win32CustomControl::Win32CustomControlProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Win32CustomControl * refClass = reinterpret_cast<Win32CustomControl*>(GetWindowLongPtr(hwnd,GWLP_USERDATA));
	switch (uMsg) {
	case WM_ACTIVATE:
		if (refClass->onActivate())
			return 0;
		break;
	case WM_DESTROY:
		if (refClass->onDestroy())
			return 0;
		break;
	case WM_KILLFOCUS:
		if (refClass->onKillFocus())
			return 0;
		break;
	case WM_SETFOCUS:
		if (refClass->onSetFocus())
			return 0;
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hwnd,&ps);
		if (refClass->onPaint(hwnd, ps))
			return 0;
		EndPaint(hwnd,&ps);
		break;
	case WM_TIMER:
		if (refClass->onTimer(wParam))
			return 0;
		break;
	}
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

void Win32CustomControl::__initializeWndClass() {
	WNDCLASS wnd =  {};

	wnd.style = CS_HREDRAW | CS_VREDRAW;
	wnd.lpfnWndProc = Win32CustomControlProc;
	wnd.hInstance = GetModuleHandle(NULL);
	wnd.hCursor = LoadCursor(NULL,IDC_ARROW);
	wnd.lpszClassName = __CLASSNAME.c_str();

	RegisterClass(&wnd);
}

void Win32CustomControl::__destroyWndClass() {
	UnregisterClass(__CLASSNAME.c_str(),GetModuleHandle(NULL));
}

const HWND& Win32CustomControl::_getParentWindow() {
	return __hwParent;
}
const RECT Win32CustomControl::_getBounds() {
	RECT r;
	GetClientRect(_getWindow(), &r);
	return r;
}

const HWND& Win32CustomControl::_getWindow() {
	return __hwnd;
}