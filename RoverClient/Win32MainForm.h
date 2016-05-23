#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif
#include <boost/asio.hpp>

#include <Windows.h>
#include <string>

#include "Win32InputRequest.h"
#include "Win32GifAnimation.h"

#include "IAppDelegate.h"
#include "resource.h"

#ifndef __WIN32MAINFORM__
#define __WIN32MAINFORM__

class Win32MainForm {
public:
	Win32MainForm(IAppDelegate &appDelegate);
	~Win32MainForm();
	void CreateForm();
protected:
	static int CALLBACK MainFormProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void _MainFormClose(void);
	void _MenuItemClick(UINT MenuItem);
	void _ConnectToRover(const std::string &ipAddress);
	void _SetStateDisconnected();
	void _SetStateConnected();
	void _SetStateConnecting();
private:
	IAppDelegate * __appDelegate;
	HWND __hdlg;
	Win32GifAnimation __waitSpinner;
};

#endif