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
		/* Dialog Boxes do not recieve WM_KEYDOWN or WM_KEYUP
		   So we have to process this message here and send it
		   Up to our dialog box - Dialog boxes also do not 
		   recieve messages about their button states - They
		   DO recieve the command but do not get notified of 
		   State change - so we look for Mouse L downs and Ups
		   and poll the button state based on that            */
		if ((msg.message == WM_KEYDOWN && !(msg.lParam & 0x40000000)) || (msg.message == WM_KEYUP && (msg.lParam & 0x40000000))) { 
			if (__mainForm.KeyMessage(msg, (msg.message == WM_KEYDOWN ? true : false))){
				__mainForm.PollButtonStates();
			}
		}
		/* The following is hack - windows will pass this message before the button actually depresses
			so it's up to us to get it and press the button before we poll the button states*/
		else if (msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONDBLCLK || msg.message == WM_LBUTTONUP) {
			msg.wParam = VK_LBUTTON;
			if (__mainForm.KeyMessage(msg, (msg.message == WM_LBUTTONUP ? false : true))) {
				__mainForm.PollButtonStates();
			}
		}
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
	requestDisconnect();
	PostQuitMessage(0);
}

void Win32RoverClient::log(const char * message) {
	__log.log(message);
}

void Win32RoverClient::onClientConnecting() {
	__mainForm.SetStateConnecting();
}

void Win32RoverClient::onClientConnect() {
	__mainForm.SetStateConnected();
}

void Win32RoverClient::onClientDisconnect() {
	__mainForm.SetStateDisconnected();
}

void Win32RoverClient::onFailedToConnect() {
	__mainForm.SetStateFailedToConnect();
}

void Win32RoverClient::requestConnection(const std::string &ipAddress, int port) {
	__rnc.ConnectToClient(ipAddress,port);
}

void Win32RoverClient::requestDisconnect() {
	if (__rnc.isConnected())
		__rnc.Disconnect();
}

void Win32RoverClient::commandRequest(const ROVERMESSAGE &rm) {
	if (!__rnc.isConnected()) return;
	__rnc.sendRoverCommand(rm);
}

void Win32RoverClient::onMessageRecieve(const ROVERMESSAGE &rm) {
	switch (rm.cmd) {
	case ROVER_RESPONSE_TRIM:
		__mainForm.SetTrimSpeed(rm.data.sWord.high, rm.data.sWord.low);
		break;
	}
}