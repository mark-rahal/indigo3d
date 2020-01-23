#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib") // link d3d11 lib
#pragma comment(lib, "D3DCompiler.lib")

Graphics::Graphics(HWND hWnd) {
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {0};
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
	struct Vertex {
		float x;
		float y;
	};

	const Vertex vertices[] = {
		{0.0f, 0.0f},
		{0.5f, 0.0f},
		{0.0f, 0.5f}
	};

	ID3D11Buffer* vertexBuffer;
	D3D11_BUFFER_DESC bufferDesc = {0};
	D3D11_SUBRESOURCE_DATA sd = {0};
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.StructureByteStride = sizeof(Vertex);
	sd.pSysMem = vertices;
	pD3DDevice->CreateBuffer(&bufferDesc, &sd, &vertexBuffer);

	// bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// create pixel shader
	ID3D11PixelShader* pPixelShader;
	ID3DBlob* pBlob;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pD3DDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	// bind pixel shader to pipeline
	pImmediateContext->PSSetShader(pPixelShader, nullptr, 0);

	// create vertex shader
	ID3D11VertexShader* pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	pD3DDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	
	// bind vertex shader to pipeline
	pImmediateContext->VSSetShader(pVertexShader, nullptr, 0);

	// input layout
	ID3D11InputLayout* pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC iED[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	pD3DDevice->CreateInputLayout(iED, 3, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

	// bind input layout
	pImmediateContext->IASetInputLayout(pInputLayout);

	// specify render target
	pImmediateContext->OMSetRenderTargets(1, &pRenderTarget, nullptr);

	// set primitive topology to triangle list
	pImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// setup viewport
	D3D11_VIEWPORT vp;
	vp.Width = 640;
	vp.Height = 480;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pImmediateContext->RSSetViewports(1, &vp);
	
	pImmediateContext->Draw(3, 0);
}