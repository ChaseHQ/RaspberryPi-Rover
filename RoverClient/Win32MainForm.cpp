#include "Win32MainForm.h"

#include "Win32InputRequest.h"

Win32MainForm::Win32MainForm(IAppDelegate * appDelegate) : __appDelegate(appDelegate) {
	__hdlg = CreateDialog(NULL,MAKEINTRESOURCE(IDD_MAINFORM),appDelegate->getParent(),reinterpret_cast<DLGPROC>(MainFormProc));
	SetWindowLongPtr(__hdlg,GWLP_USERDATA,reinterpret_cast<LONG>(this));
	__appDelegate->log("Win32MainForm::Win32MainForm() Instantiated");
}

Win32MainForm::~Win32MainForm() {
	__appDelegate->log("Win32MainForm::~Win32MainForm() Destroyed");
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
	case ID_ROVER_EXIT:
		_MainFormClose();
		break;
	case ID_ROVER_CONNECT:
		Win32InputRequest request(__hdlg, std::string("Rover IP Address"), [&](bool okPressed, const std::string &inputString) {
			if (okPressed)
				this->_ConnectToRover(inputString);
		});
		break;
	}
}

void Win32MainForm::_ConnectToRover(const std::string &ipAddress) {
	
}