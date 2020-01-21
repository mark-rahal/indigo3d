#pragma once
#include "DisableWindowsStuff.h"
#include <d3d11.h>

class Graphics {
private:
	ID3D11Device* pD3DDevice;
	IDXGISwapChain* pDXSwapChain;
	ID3D11DeviceContext* pImmediateContext;
	ID3D11RenderTargetView* pRenderTarget;
public:
	// provide the graphics class with a handle to our window
	Graphics(HWND);
	~Graphics();
	void endFrame();
	void clearBackBuffer(float red, float green, float blue);
	void drawTriangle();
};