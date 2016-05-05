#include <Windows.h>
#include <boost\shared_ptr.hpp>

#include "Win32RoverLog.h"
#include "IAppDelegate.h"
#include "Win32MainForm.h"

#ifndef __WIN32ROVERCLIENT__
#define __WIN32ROVERCLIENT__

class Win32RoverClient : public IAppDelegate {
public:
	Win32RoverClient (HINSTANCE hInstance);
	~Win32RoverClient();
	void run();
	void onMainWindowClose();
	HWND getParent(void);
protected:
	void _initializeMainWindow();
	void _createMainForm();
	static LRESULT CALLBACK RoverWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	HINSTANCE __hInstance;
	HWND __hWindow;
	Win32RoverLog __log;
	boost::shared_ptr<Win32MainForm> __mainForm;
};

#endif