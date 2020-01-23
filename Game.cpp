#include "Game.h"

Game::Game() : window(640, 480, L"Direct3D Window"), gfx(window.getWindowHandle()) {

}

Game::~Game() {

}

int Game::run() {
	// message loop
	MSG msg;
	BOOL getMessageResult{};
	while (GetMessage(&msg, nullptr, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		render();
	}

	// error handling for GetMessage() call
	if (getMessageResult == -1) {
		return -1;
	}

	return msg.wParam;
}

void Game::render() {
	gfx.clearBackBuffer(255.0f, 0.0f, 0.0f);
	gfx.drawTriangle();
	gfx.endFrame();
}