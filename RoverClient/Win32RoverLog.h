#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <sstream>

#include "resource.h"

#ifndef __WIN32ROVERLOG__
#define __WIN32ROVERLOG__

class Win32RoverLog {
public:
	Win32RoverLog();
	~Win32RoverLog();
	void log(const char * LogMsg);
protected:
	static int CALLBACK DebugBoxProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	HWND __hdlg;
};

#endif