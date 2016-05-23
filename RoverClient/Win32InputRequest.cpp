#include "Win32InputRequest.h"

Win32InputRequest::Win32InputRequest(HWND owner, const std::string &title, std::function<bool(bool okPressed, const std::string &inputString,const HWND &dlgHandle)> inputReturn) :
	__title(title), __inputReturn(inputReturn)
{
	DialogBoxParam(NULL,MAKEINTRESOURCE(IDD_INPUTREQ),owner,reinterpret_cast<DLGPROC>(InputRequestProc),reinterpret_cast<LPARAM>(this));
}

Win32InputRequest::~Win32InputRequest() {

}

void Win32InputRequest::_SetupInputRequestDialog(HWND hDlg) {
	__hdlg = hDlg;
	SetWindowText(hDlg,__title.c_str());
}

void Win32InputRequest::_SetReturnParams(bool buttonPressed,const std::string &response) {
	if (__inputReturn(buttonPressed,response,__hdlg))
		EndDialog(__hdlg,0);
}

int CALLBACK Win32InputRequest::InputRequestProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Win32InputRequest * refDialog = reinterpret_cast<Win32InputRequest *> (GetWindowLongPtr(hDlg,GWLP_USERDATA));
	switch (uMsg) {
		case WM_INITDIALOG:
			SetWindowLongPtr(hDlg,GWLP_USERDATA,lParam);
			reinterpret_cast<Win32InputRequest *>(lParam)->_SetupInputRequestDialog(hDlg);
			ShowWindow(hDlg,SW_SHOW);
			break;
		case WM_CLOSE:
			break;
		case WM_COMMAND:
			std::vector<char> buffer(Edit_GetTextLength(GetDlgItem(hDlg,IDC_IR_EDIT))+1);
			Edit_GetText(GetDlgItem(hDlg,IDC_IR_EDIT),buffer.data(),buffer.size());
			switch (LOWORD(wParam)) {
			case ID_IR_OK:
				refDialog->_SetReturnParams(true,std::string(buffer.begin(),buffer.end()));
				break;
			case ID_IR_CANCEL:
				refDialog->_SetReturnParams(false,std::string(buffer.begin(),buffer.end()));
				break;
			}
			return 0;
			break;
	}

	return false;
}