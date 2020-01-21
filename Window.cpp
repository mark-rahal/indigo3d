#include "Window.h"

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() : hInst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getClassName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(getClassName(), getInstance());
}

Window::Window(int width, int height, const LPCWSTR title) {
	hWnd = CreateWindowEx(0, WindowClass::getClassName(), title, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 200, 200, width, height, nullptr, nullptr, WindowClass::getInstance(), nullptr);
	ShowWindow(hWnd, SW_SHOW);
}

Window::~Window() {
	DestroyWindow(hWnd);
}

const LPCWSTR Window::WindowClass::getClassName() {
	return windowClass.wndClassName;
}

HINSTANCE Window::WindowClass::getInstance() {
	return windowClass.hInst;
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	// encapsulate these in keyboard/mouse event class instead of directly handling them here
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == 'F') {
			SetWindowText(hWnd, L"F");
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND Window::getWindowHandle() {
	return hWnd;
}