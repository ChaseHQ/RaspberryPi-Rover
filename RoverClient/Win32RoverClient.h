#include <Windows.h>
#include <boost\smart_ptr.hpp>
#include <string>

#include "Win32RoverLog.h"
#include "IAppDelegate.h"
#include "RoverNetworkClient.h"
#include "Win32MainForm.h"

#ifndef __WIN32ROVERCLIENT__
#define __WIN32ROVERCLIENT__

class Win32RoverClient : public IAppDelegate, public IRoverNetworkClientDelegate {
public:
	Win32RoverClient (HINSTANCE hInstance);
	~Win32RoverClient();
	void run();
	// IAppDelegate
	void onMainWindowClose();
	void log(const char * message);
	void requestConnection(const std::string &ipAddress, int port);
	// IRoverNetworkClientDelegate
	void onClientConnect();
	void onClientDisconnect();
protected:
	void _initializeMainWindow();
	void _createMainForm();
	static LRESULT CALLBACK RoverWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	HINSTANCE __hInstance;
	HWND __hWindow;
	Win32RoverLog __log;
	Win32MainForm __mainForm;
	RoverNetworkClient __rnc;
};

#endif