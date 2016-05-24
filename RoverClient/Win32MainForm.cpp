#include "Win32MainForm.h"

Win32MainForm::Win32MainForm(IAppDelegate &appDelegate) : __appDelegate(&appDelegate) {
	
}

Win32MainForm::~Win32MainForm() {
	__appDelegate->log("Win32MainForm::~Win32MainForm() Destroyed");
}

void Win32MainForm::CreateForm() {
	__hdlg = CreateDialog(NULL,MAKEINTRESOURCE(IDD_MAINFORM),NULL,reinterpret_cast<DLGPROC>(MainFormProc));
	SetWindowLongPtr(__hdlg,GWLP_USERDATA,reinterpret_cast<LONG>(this));
	__appDelegate->log("Win32MainForm::Win32MainForm() Instantiated");
	RECT r;
	GetClientRect(__hdlg, &r);
	r.top = (r.bottom / 2) - (246 / 2);
	r.left = (r.right / 2) - (246 / 2);
	r.right = 246;
	r.bottom = 246;
	__waitSpinner.SetImageFromResourceName(IDR_WAIT);
	__waitSpinner.CreateControl(__hdlg,r);
	__waitSpinner.StartAnimating(50);
	__waitSpinner.SetVisible(false);
}

void Win32MainForm::_MainFormClose() {
	__appDelegate->log("Win32MainForm::MainFormClose() Closing Main Form");
	EndDialog(__hdlg,0);
	__appDelegate->onMainWindowClose();
}

int CALLBACK Win32MainForm::MainFormProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Win32MainForm * refDialog = reinterpret_cast<Win32MainForm *> (GetWindowLongPtr(hDlg,GWLP_USERDATA));
	switch (uMsg) {
		case WM_INITDIALOG:
			ShowWindow(hDlg,SW_SHOW);
			break;
		case WM_CLOSE:
			refDialog->_MainFormClose();
			break;
		case WM_COMMAND:
			refDialog->_MenuItemClick(LOWORD(wParam));
			return 0;
			break;
	}

	return false;
}

void Win32MainForm::_MenuItemClick(UINT MenuItem) {
	switch (MenuItem) {
	case ID_DEBUG_HIDEANIMATION:
		__waitSpinner.SetVisible(!__waitSpinner.GetVisible());
		break;
	case ID_DEBUG_PAUSEANIMATION:
		if (__waitSpinner.IsAnimating())
			__waitSpinner.StopAnimating();
		else
			__waitSpinner.StartAnimating(50);
		break;
	case ID_ROVER_EXIT:
		_MainFormClose();
		break;
	case ID_ROVER_CONNECT:
		Win32InputRequest request(__hdlg, std::string("Rover IP Address"), [&](bool okPressed, const std::string &inputString, const HWND &dlgHandle)->bool {
			if (okPressed) {
				// Error Check that this is an IP address -
				boost::system::error_code ec;
				boost::asio::ip::address_v4::from_string(inputString,ec);
				if (ec.value()) {
					MessageBox(dlgHandle,"This is an invalid IP Address","IP Address",MB_ICONERROR);
					return false;
				} else 
					this->_ConnectToRover(inputString);
			}
			return true;
		});
		break;
	}
}

void Win32MainForm::_ConnectToRover(const std::string &ipAddress) {
	_SetStateConnecting();
	__appDelegate->requestConnection(ipAddress,5000);
}

void Win32MainForm::_SetStateDisconnected() {

}

void Win32MainForm::_SetStateConnected() {

}

void Win32MainForm::_SetStateConnecting() {
	_SetStateDisconnected();

}