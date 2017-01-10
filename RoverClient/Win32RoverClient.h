#include "Win32MainForm.h"

#include <Windows.h>
#include <boost\smart_ptr.hpp>
#include <string>

#include "Win32RoverLog.h"
#include "IAppDelegate.h"
#include "RoverNetworkClient.h"

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
	void requestDisconnect();
	void commandRequest(const ROVERMESSAGE &rm);
	// IRoverNetworkClientDelegate
	void onClientConnect();
	void onClientDisconnect();
	void onFailedToConnect();
	void onClientConnecting();
	void onMessageRecieve(const ROVERMESSAGE &rm);
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