#include "Game.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// TODO: Add error handling/messages using HRESULT
	int exitCode = Game{}.run();

	return exitCode;
}