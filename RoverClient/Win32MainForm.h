#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <Windows.h>
#include <Commctrl.h>
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
	void SetStateDisconnected();
	void SetStateConnected();
	void SetStateConnecting();
	void SetStateFailedToConnect();
	void SetTrimSpeed(int iLTrimSpeed, int iRTrimSpeed);
	bool KeyMessage(const MSG &msg, bool keyDown);
	void PollButtonStates();
protected:
	static int CALLBACK MainFormProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void _MainFormClose(void);
	void _MenuItemClick(UINT MenuItem);
	void _TrackBarChange(HWND hTrackBar, bool bFinal);
	void _ConnectToRover(const std::string &ipAddress);
	IStream * _StreamFromResource(UINT uiResource, const LPCSTR lpLocation);
	void _AssignIconToButton(UINT uiButtonId, UINT uiIconName, const LPCSTR lpIconLocation);
	bool _IsMouseOverButton(UINT uiButtonId);
	void _RequestInitialRoverData();
private:
	IAppDelegate * __appDelegate;
	HWND __hdlg;
	Win32GifAnimation __waitSpinner;
	bool __stateConnected;
};

#endif