#include <Windows.h>
#include <windowsx.h>

#include <functional>
#include "resource.h"
#include <string>
#include <vector>

#ifndef __WIN32INPUTREQUEST__
#define __WIN32INPUTREQUEST__

class Win32InputRequest {
public:
	Win32InputRequest(HWND owner, const std::string &title, std::function<bool(bool okPressed, const std::string &inputString,const HWND &dlgHandle)> inputReturn);
	~Win32InputRequest();
protected:
	static int CALLBACK InputRequestProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void _SetupInputRequestDialog(HWND hDlg);
	void _SetReturnParams(bool buttonPressed, const std::string &response);
private:
	std::string __title;
	HWND __hdlg;
	std::function<bool(bool okPressed, const std::string &inputString,const HWND &dlgHandle)> __inputReturn;
};

#endif