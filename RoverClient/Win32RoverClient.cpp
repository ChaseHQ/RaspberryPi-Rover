#include "Win32RoverClient.h"

Win32RoverClient::Win32RoverClient(HINSTANCE hInstance) : __hInstance(hInstance), __rnc(*this), __mainForm(*this) {
	_initializeMainWindow();
}

Win32RoverClient::~Win32RoverClient() {
	
}

void Win32RoverClient::_initializeMainWindow() {
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = Win32RoverClient::RoverWindowProc;
	wndClass.hInstance = __hInstance;
	wndClass.lpszClassName = "Win32RoverClient";

	RegisterClassEx(&wndClass);

	__hWindow = CreateWindow("Win32RoverClient","Win32RoverClient",0,0,0,0,0,HWND_MESSAGE,NULL,__hInstance,NULL);

	if (!__hWindow) {
		log("Could Not Create Win32RoverClient Message Window");
	} else {
		log("Win32RoverClient Message Handler Active");
	}

	SetWindowLongPtr(__hWindow,GWLP_USERDATA,reinterpret_cast<LONG>(this));

	_createMainForm();
}

void Win32RoverClient::run() {
	MSG msg;
	while (GetMessage(&msg,NULL,NULL,NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	log("Win32RoverClient::run() Ending");
}

void Win32RoverClient::_createMainForm() {
	__mainForm.CreateForm();
}

LRESULT CALLBACK Win32RoverClient::RoverWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Win32RoverClient * refWindow = reinterpret_cast<Win32RoverClient *> (GetWindowLongPtr(hwnd,GWLP_USERDATA));
	switch (uMsg) {
	case WM_QUIT:

		break;
	}
	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

void Win32RoverClient::onMainWindowClose() {
	log("Win32RoverClient::onMainWindowClose() Called");
	PostQuitMessage(0);
}

void Win32RoverClient::log(const char * message) {
	__log.log(message);
}

void Win32RoverClient::onClientConnect() {

}

void Win32RoverClient::onClientDisconnect() {

}

void Win32RoverClient::requestConnection(const std::string &ipAddress, int port) {
	__rnc.ConnectToClient(ipAddress,port);
}