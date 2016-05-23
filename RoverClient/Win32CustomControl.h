#include <Windows.h>
#include <windowsx.h>
#include <mutex>
#include <atomic>
#include <string>

#ifndef __WIN32CUSTOMCONTROL__
#define __WIN32CUSTOMCONTROL__

class Win32CustomControl {
public:
	~Win32CustomControl();
	void CreateControl(const HWND &parent, const RECT &bounds); 
protected:
	Win32CustomControl(const char * className);
	static LRESULT CALLBACK Win32CustomControlProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	const HWND& _getParentWindow();
	const RECT _getBounds();
	const HWND& _getWindow();
	// Abstract Method to Get Win32 Class Name
	virtual bool onPaint(const HWND& hWnd, const PAINTSTRUCT& ps) { return false; };
	virtual bool onDestroy() { return false; };
	virtual bool onActivate() { return false; };
	virtual bool onSetFocus() { return false; };
	virtual bool onKillFocus() { return false; };
	virtual bool onTimer(const WPARAM& wParam) { return false; };
private:
	std::mutex __mtx;
	static std::atomic_uint __CLASSCOUNT;
	std::string __CLASSNAME;
	void __initializeWndClass();
	void __destroyWndClass();
	HWND __hwnd;
	HWND __hwParent;
};

#endif