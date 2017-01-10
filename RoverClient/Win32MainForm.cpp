#include "Win32MainForm.h"

Win32MainForm::Win32MainForm(IAppDelegate &appDelegate) : __appDelegate(&appDelegate), __stateConnected(false) {
	
}

Win32MainForm::~Win32MainForm() {
	__appDelegate->log("Win32MainForm::~Win32MainForm() Destroyed");
}

void Win32MainForm::CreateForm() {
	INITCOMMONCONTROLSEX icomex = {};
	icomex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icomex.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icomex);

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
	
	_AssignIconToButton(IDC_BUP, IDB_UP, "PNG");
	_AssignIconToButton(IDC_BDOWN, IDB_DOWN, "PNG");
	_AssignIconToButton(IDC_BLEFT, IDB_LEFT, "PNG");
	_AssignIconToButton(IDC_BRIGHT, IDB_RIGHT, "PNG");
	_AssignIconToButton(IDC_BHORN, IDB_HORN, "PNG");

	SendDlgItemMessage(__hdlg, IDC_LTRIM, TBM_SETRANGEMAX, true, 127);
	SendDlgItemMessage(__hdlg, IDC_RTRIM, TBM_SETRANGEMAX, true, 127);
	_TrackBarChange(GetDlgItem(__hdlg, IDC_LTRIM), false);
	_TrackBarChange(GetDlgItem(__hdlg, IDC_RTRIM), false);

	SetStateDisconnected();
}

void Win32MainForm::_AssignIconToButton(UINT uiButtonId, UINT uiIconName, const LPCSTR lpIconLocation) {
	auto stream = _StreamFromResource(uiIconName, lpIconLocation);
	HICON hicon = nullptr;
	auto m_pBitmap = Gdiplus::Bitmap::FromStream(stream);
	m_pBitmap->GetHICON(&hicon);
	SendDlgItemMessage(__hdlg, uiButtonId, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hicon);
	stream->Release();
	delete m_pBitmap;
}

IStream * Win32MainForm::_StreamFromResource(UINT uiResource, const LPCSTR lpLocation) {
	IStream * stream = nullptr;

	HINSTANCE hInst = GetModuleHandle(0);
	HRSRC hC = FindResource(hInst, MAKEINTRESOURCE(uiResource), lpLocation);
	HGLOBAL hG = LoadResource(hInst, hC);
	void * bytes = LockResource(hG);
	ULONG size = SizeofResource(hInst, hC);
	HRESULT hr = CreateStreamOnHGlobal(NULL, true, &stream);
	ULONG written;
	hr = stream->Write(bytes, size, &written);

	return stream;
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
		case WM_VSCROLL:
			switch LOWORD(wParam) {
			case TB_ENDTRACK:
				refDialog->_TrackBarChange(reinterpret_cast<HWND>(lParam), true);
				break;
			case TB_THUMBTRACK:
				refDialog->_TrackBarChange(reinterpret_cast<HWND>(lParam), false);
				break;
			}
			break;
		case WM_COMMAND:
			switch (HIWORD(wParam)) {
			case 0: // Menu
				refDialog->_MenuItemClick(LOWORD(wParam));
				break;
			}
			return 0;
			break;
	}

	return false;
}

void Win32MainForm::_TrackBarChange(HWND hTrackBar, bool bFinal){
	int tbPos = SendMessage(hTrackBar, TBM_GETPOS, 0, 0);
	switch (GetWindowLong(hTrackBar, GWL_ID)) {
	case IDC_LTRIM:
		SendDlgItemMessage(__hdlg, IDC_LVAL, WM_SETTEXT, 0, reinterpret_cast<LPARAM> (std::to_string(tbPos).c_str()));
		break;
	case IDC_RTRIM:
		SendDlgItemMessage(__hdlg, IDC_RVAL, WM_SETTEXT, 0, reinterpret_cast<LPARAM> (std::to_string(tbPos).c_str()));
		break;
	}
	if (bFinal) {
		ROVERMESSAGE rm;
		rm.cmd = ROVER_SETTRIM;
		rm.data.sWord.high = SendMessage(GetDlgItem(__hdlg,IDC_LTRIM), TBM_GETPOS, 0, 0);
		rm.data.sWord.low = SendMessage(GetDlgItem(__hdlg, IDC_RTRIM), TBM_GETPOS, 0, 0);
		__appDelegate->commandRequest(&rm);
	}
}

bool Win32MainForm::KeyMessage(const MSG &msg, bool keyDown) {
	if (!__stateConnected) return false;
	switch (msg.wParam) {
	case VK_DOWN:
		Button_SetState(GetDlgItem(__hdlg, IDC_BDOWN), keyDown);
		return true;
		break;
	case VK_UP:
		Button_SetState(GetDlgItem(__hdlg, IDC_BUP), keyDown);
		return true;
		break;
	case VK_LEFT:
		Button_SetState(GetDlgItem(__hdlg, IDC_BLEFT), keyDown);
		return true;
		break;
	case VK_RIGHT:
		Button_SetState(GetDlgItem(__hdlg, IDC_BRIGHT), keyDown);
		return true;
		break;
	case 0x48:
		Button_SetState(GetDlgItem(__hdlg, IDC_BHORN), keyDown);
		return true;
		break;
	case VK_LBUTTON:
		// The button state isn't actually down here - We need to make sure that it does show down
		// once this method ends it will poll the states of the buttons
		// We need to check to see if the mouse is over the button - then react accordingly
		for (int butId = 1010; butId <= 1014; ++butId) { // Hacky hack with the resource ID #'s :(
			if (_IsMouseOverButton(butId)) {
				Button_SetState(GetDlgItem(__hdlg, butId), keyDown);
			}
		}
		return true;
		break;
	}
	return false;
}

bool Win32MainForm::_IsMouseOverButton(UINT uiButtonId) {
	POINT pt;
	RECT rct;
	GetCursorPos(&pt);
	GetWindowRect(GetDlgItem(__hdlg, uiButtonId), &rct);
	return PtInRect(&rct, pt);
}

void Win32MainForm::PollButtonStates() {
	// This is called when there could possibly be a change in button state - Check and act on
	if (!__stateConnected) return; // Make sure we are actually connected
	__appDelegate->log("Poll Button State Requested");
	auto roverMessage = boost::shared_ptr<ROVERMESSAGE>(new ROVERMESSAGE());
	if (Button_GetState(GetDlgItem(__hdlg, IDC_BDOWN)) & BST_PUSHED){
		roverMessage->cmd = ROVER_BACK;
	}
	else if (Button_GetState(GetDlgItem(__hdlg, IDC_BUP)) & BST_PUSHED) {
		roverMessage->cmd = ROVER_FORWARD;
	}
	else if (Button_GetState(GetDlgItem(__hdlg, IDC_BLEFT)) & BST_PUSHED) {
		roverMessage->cmd = ROVER_LEFT;
	}
	else if (Button_GetState(GetDlgItem(__hdlg, IDC_BRIGHT)) & BST_PUSHED) {
		roverMessage->cmd = ROVER_RIGHT;
	}
	else if (Button_GetState(GetDlgItem(__hdlg, IDC_BHORN)) & BST_PUSHED) {
		roverMessage->cmd = ROVER_HORN;
	}
	else {
		// No buttons down - must be a stop
		roverMessage->cmd = ROVER_STOP;
	}
	__appDelegate->commandRequest(roverMessage.get());
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
	case ID_ROVER_DISCONNECT:
		__appDelegate->requestDisconnect();
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
	__appDelegate->requestConnection(ipAddress,5000);
}

void Win32MainForm::SetTrimSpeed(int iLTrimSpeed, int iRTrimSpeed) {
	SendDlgItemMessage(__hdlg, IDC_LTRIM, TBM_SETPOS, true, iLTrimSpeed);
	SendDlgItemMessage(__hdlg, IDC_RTRIM, TBM_SETPOS, true, iRTrimSpeed);
	_TrackBarChange(GetDlgItem(__hdlg, IDC_LTRIM), false);
	_TrackBarChange(GetDlgItem(__hdlg, IDC_RTRIM), false);
}

void Win32MainForm::SetStateDisconnected() {
	EnableMenuItem(GetMenu(__hdlg), ID_ROVER_CONNECT, MF_ENABLED);
	EnableMenuItem(GetMenu(__hdlg), ID_ROVER_DISCONNECT, MF_GRAYED);
	Button_Enable(GetDlgItem(__hdlg, IDC_BUP), false);
	Button_Enable(GetDlgItem(__hdlg, IDC_BDOWN), false);
	Button_Enable(GetDlgItem(__hdlg, IDC_BLEFT), false);
	Button_Enable(GetDlgItem(__hdlg, IDC_BRIGHT), false);
	Button_Enable(GetDlgItem(__hdlg, IDC_BHORN), false);
	EnableWindow(GetDlgItem(__hdlg, IDC_LTRIM), false);
	EnableWindow(GetDlgItem(__hdlg, IDC_RTRIM), false);
	__stateConnected = false;
}

void Win32MainForm::SetStateConnected() {
	// First Retrieve Needed Data from Rover
	_RequestInitialRoverData();
	// Now Continue on with giving control to end user
	EnableMenuItem(GetMenu(__hdlg), ID_ROVER_DISCONNECT, MF_ENABLED);
	__waitSpinner.SetVisible(false);
	__waitSpinner.StopAnimating();
	Button_Enable(GetDlgItem(__hdlg, IDC_BUP), true);
	Button_Enable(GetDlgItem(__hdlg, IDC_BDOWN), true);
	Button_Enable(GetDlgItem(__hdlg, IDC_BLEFT), true);
	Button_Enable(GetDlgItem(__hdlg, IDC_BRIGHT), true);
	Button_Enable(GetDlgItem(__hdlg, IDC_BHORN), true);
	EnableWindow(GetDlgItem(__hdlg, IDC_LTRIM), true);
	EnableWindow(GetDlgItem(__hdlg, IDC_RTRIM), true);
	__stateConnected = true;
}

void Win32MainForm::_RequestInitialRoverData() {
	ROVERMESSAGE rm;
	rm.cmd = ROVER_GETTRIM;
	__appDelegate->commandRequest(rm);
}

void Win32MainForm::SetStateConnecting() {
	__waitSpinner.StartAnimating(50);
	__waitSpinner.SetVisible(true);
	EnableMenuItem(GetMenu(__hdlg), ID_ROVER_CONNECT, MF_GRAYED);
}

void Win32MainForm::SetStateFailedToConnect() {
	__waitSpinner.SetVisible(false);
	__waitSpinner.StopAnimating();
	SetStateDisconnected();
	MessageBox(__hdlg, "Failed to Connect to Server", "Connection Failure", MB_ICONERROR);
}