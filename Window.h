#pragma once
#include "DisableWindowsStuff.h"

class Window {
private:
	class WindowClass {
	private:
		WindowClass();
		~WindowClass();
		static constexpr const LPCWSTR wndClassName = L"Direct3D Window";
		static WindowClass windowClass;
		HINSTANCE hInst;
	public:
		static const LPCWSTR getClassName();
		static HINSTANCE getInstance();
	};
	HWND hWnd;
	static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
public:
	Window(int, int, const LPCWSTR);
	~Window();
	HWND getWindowHandle();
	};
	