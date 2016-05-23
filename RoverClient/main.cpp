#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#endif
#include <boost/asio.hpp>

#include <Windows.h>
#include "Win32RoverClient.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {

	Win32RoverClient client(hInstance);

	client.run();

	return 0;
}