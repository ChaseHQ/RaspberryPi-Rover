#include "Win32RoverLog.h"

Win32RoverLog::Win32RoverLog() {
	__hdlg = CreateDialog(NULL,MAKEINTRESOURCE(IDD_ROVERLOG),NULL,reinterpret_cast<DLGPROC>(DebugBoxProc));
	SetWindowLongPtr(__hdlg,GWLP_USERDATA,reinterpret_cast<LONG>(this));
	log("RoverLog Instantiated");
}

Win32RoverLog::~Win32RoverLog() {
	EndDialog(__hdlg,0);
}

void Win32RoverLog::log(const char * LogMsg) {
	SYSTEMTIME st = {};
	std::stringstream stringBuf;
	char * buffer = NULL;

	GetSystemTime(&st); // get the system time
	
	int bufSize = Edit_GetTextLength(GetDlgItem(__hdlg,IDC_LOGBOX)); // Get the Buffer Window Size
	buffer = (char *) malloc((bufSize + 2) * sizeof(char)); // set the Buffer Large Enough to hold the text
	
	Edit_GetText(GetDlgItem(__hdlg,IDC_LOGBOX),buffer, bufSize + 1); // get the text

	stringBuf << buffer  << "[" << st.wHour << ":" << st.wMinute << ":"
		<< ":" << st.wSecond << ":" << st.wMilliseconds << "] - " << LogMsg << "\r\n"; // Append The String Buf with the loginfo & systime

	Edit_SetText(GetDlgItem(__hdlg,IDC_LOGBOX),stringBuf.str().c_str()); // Set the String in

	Edit_Scroll(GetDlgItem(__hdlg,IDC_LOGBOX),Edit_GetLineCount(GetDlgItem(__hdlg,IDC_LOGBOX)),0); // Scroll to the last row
}

int CALLBACK Win32RoverLog::DebugBoxProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Win32RoverLog * refDialog = reinterpret_cast<Win32RoverLog *> (GetWindowLongPtr(hDlg,GWLP_USERDATA));
	switch (uMsg) {
		case WM_INITDIALOG:
			ShowWindow(hDlg,SW_SHOW);
			break;
	}

	return false;
}