#include <Windows.h>

#ifndef __IAPPDELEGATE__
#define __IAPPDELEGATE__

class IAppDelegate {
public:
	virtual void onMainWindowClose(void) = 0;
	virtual HWND getParent(void) = 0;
};

#endif