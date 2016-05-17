#include <Windows.h>
#include "ILog.h"

#ifndef __IAPPDELEGATE__
#define __IAPPDELEGATE__

class IAppDelegate : public ILog {
public:
	virtual void onMainWindowClose(void) = 0;
	virtual HWND getParent(void) = 0;
};

#endif