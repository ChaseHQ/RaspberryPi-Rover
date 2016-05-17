#include <Windows.h>
#include <string>

#include "IAppDelegate.h"

#include "resource.h"

#ifndef __WIN32MAINFORM__
#define __WIN32MAINFORM__

class Win32MainForm {
public:
	Win32MainForm(IAppDelegate * appDelegate);
	~Win32MainForm();
protected:
	static int CALLBACK MainFormProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void _MainFormClose(void);
	void _MenuItemClick(UINT MenuItem);
	void _ConnectToRover(const std::string &ipAddress);
private:
	IAppDelegate * __appDelegate;
	HWND __hdlg;
};

#endif