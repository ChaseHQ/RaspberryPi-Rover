#include <Windows.h>
#include "Win32RoverClient.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {

	Win32RoverClient client(hInstance);

	client.run();

	return 0;
}