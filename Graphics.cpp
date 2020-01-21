#include "Graphics.h"

#pragma comment(lib, "d3d11.lib") // link d3d11 lib

Graphics::Graphics(HWND hWnd) {
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1; // no anti-aliasing
	swapChainDesc.SampleDesc.Quality = 0; // no AA
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // use buffer as render target
	swapChainDesc.BufferCount = 1; // 1 back-buffer (double buffering)
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(
		nullptr, // use default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, // not using software
		0, // don't enable any runtime layers
		nullptr, // use default array for order of direct3D feature levels
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pDXSwapChain,
		&pD3DDevice,
		nullptr,
		&pImmediateContext
	);

	// get access to back buffer
	ID3D11Resource* pBackBuffer = nullptr; // TODO: Use ComPtr for resources
	pDXSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	pD3DDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTarget);
	pBackBuffer->Release();
}

Graphics::~Graphics() {
	// device creation possibly failed, check if resources exist before destroying them
	if (pImmediateContext != nullptr) {
		pImmediateContext->Release();
	}
	if (pDXSwapChain != nullptr) {
		pDXSwapChain->Release();
	}
 	if (pD3DDevice != nullptr) {
		pD3DDevice->Release();
	}
}

void Graphics::endFrame() {
	pDXSwapChain->Present(1, 0);
}

void Graphics::clearBackBuffer(float red, float green, float blue) {
	const float color[] = {red, green, blue, 1.0f};
	pImmediateContext->ClearRenderTargetView(pRenderTarget, color);
}

void Graphics::drawTriangle() {
	ID3D11Buffer* vertexBuffer;
	pD3DDevice->CreateBuffer();
	pImmediateContext->IASetVertexBuffers(0, 1);
	pImmediateContext->Draw();
}