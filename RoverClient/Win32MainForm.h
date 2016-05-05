#include <Windows.h>

#include "ILogger.h"
#include "IAppDelegate.h"

#include "resource.h"

#ifndef __WIN32MAINFORM__
#define __WIN32MAINFORM__

class Win32MainForm {
public:
	Win32MainForm(ILogger * logger, IAppDelegate * appDelegate);
	~Win32MainForm();
protected:
	static int CALLBACK MainFormProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void _MainFormClose(void);
	void _MenuItemClick(UINT MenuItem);
private:
	ILogger * __log;
	IAppDelegate * __appDelegate;
	HWND __hdlg;
};

#endif