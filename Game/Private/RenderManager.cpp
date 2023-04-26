#include <array>

#include "Window.h"
#include "RenderManager.h"

void RenderManager::Setup(Window* renderTargetWindow)
{
	if (bIsSetup_) return;

	renderTargetWindow_ = renderTargetWindow;

	CHECK_HR(CreateDeviceAndContext(), "failed to create device and context...");
	CHECK_HR(CreateSwapChain(renderTargetWindow_->GetNativeHandle()), "failed to create swapchain...");
	CHECK_HR(CreateRenderTargetView(), "failed to create render target view...");
	CHECK_HR(CreateDepthStencilView(), "failed to create depth stencil view...");

	CHECK_HR(CreateDepthStencilState(&depthStencilStates_["EnableZ"], true, true), "failed to create enable z depth stencil state");
	CHECK_HR(CreateDepthStencilState(&depthStencilStates_["DisableZ"], false, true), "failed to create disable z depth stencil state");
	CHECK_HR(CreateBlendState(&blendStates_["Alpha"], true), "failed to create alpha blend state");
	CHECK_HR(CreateRasterizerState(&rasterizerStates_["Fill"], false, true), "failed to create fill mode rasterizer state");
	CHECK_HR(CreateRasterizerState(&rasterizerStates_["Wireframe"], false, false), "failed to create wireframe mode rasterizer state");

	bIsSetup_ = true;
}

void RenderManager::Cleanup()
{
	if (!bIsSetup_) return;

	if (renderTargetWindow_->IsFullscreen() && swapChain_)
	{
		CHECK_HR(swapChain_->SetFullscreenState(false, nullptr), "failed to set full screen state...");
	}

	for (auto& rasterizerState : rasterizerStates_)
	{
		SAFE_RELEASE(rasterizerState.second);
	}

	for (auto& blendState : blendStates_)
	{
		SAFE_RELEASE(blendState.second);
	}

	for (auto& depthStencilState : depthStencilStates_)
	{
		SAFE_RELEASE(depthStencilState.second);
	}

	SAFE_RELEASE(depthStencilView_);
	SAFE_RELEASE(depthStencilBuffer_);
	SAFE_RELEASE(renderTargetView_);
	SAFE_RELEASE(swapChain_);

	if (context_)
	{
		context_->ClearState();
	}

	SAFE_RELEASE(context_);
	SAFE_RELEASE(device_);

	bIsSetup_ = false;
}

void RenderManager::BeginFrame(float red, float green, float blue, float alpha, float depth, uint8_t stencil)
{
	context_->OMSetRenderTargets(1, &renderTargetView_, depthStencilView_);

	float colorRGBA[4] = { red, green, blue, alpha };

	context_->ClearRenderTargetView(renderTargetView_, colorRGBA);
	context_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void RenderManager::EndFrame(bool bIsVSync)
{
	CHECK_HR(swapChain_->Present(static_cast<uint32_t>(bIsVSync), 0), "failed to present backbuffer...");
}

void RenderManager::Resize()
{
	int32_t backBufferWidth = 0;
	int32_t backBufferHeight = 0;
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	uint32_t backBufferCount = 2;

	SAFE_RELEASE(renderTargetView_);

	renderTargetWindow_->GetSize(backBufferWidth, backBufferHeight);

	CHECK_HR(
		swapChain_->ResizeBuffers(backBufferCount, static_cast<uint32_t>(backBufferWidth), static_cast<uint32_t>(backBufferHeight), backBufferFormat, 0),
		"failed to resize buffer..."
	);
	CHECK_HR(CreateRenderTargetView(), "failed to create render target view...");
	CHECK_HR(CreateDepthStencilView(), "failed to create depth stencil view...");
}

void RenderManager::SetViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = topLeftX;
	viewport.TopLeftY = topLeftY;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = minDepth;
	viewport.MaxDepth = maxDepth;

	context_->RSSetViewports(1, &viewport);
}

void RenderManager::SetWindowViewport(float minDepth, float maxDepth)
{
	int32_t windowWidth = 0;
	int32_t windowHeight = 0;
	renderTargetWindow_->GetSize(windowWidth, windowHeight);

	SetViewport(0.0f, 0.0f, static_cast<float>(windowWidth), static_cast<float>(windowHeight), minDepth, maxDepth);
}

void RenderManager::SetDepthBuffer(bool bIsEnable)
{
	ID3D11DepthStencilState* depthStencilState = bIsEnable ? depthStencilStates_["EnableZ"] : depthStencilStates_["DisableZ"];

	context_->OMSetDepthStencilState(depthStencilState, 1);
}

void RenderManager::SetAlphaBlend(bool bIsEnable)
{
	ID3D11BlendState* blendState = bIsEnable ? blendStates_["Alpha"] : nullptr;

	context_->OMSetBlendState(blendState, nullptr, 0xFFFFFFFF);
}

void RenderManager::SetFillMode(bool bIsEnable)
{
	ID3D11RasterizerState* rasterizerState = bIsEnable ? rasterizerStates_["Fill"] : rasterizerStates_["Wireframe"];

	context_->RSSetState(rasterizerState);
}

RenderManager::~RenderManager()
{
	Cleanup();
}

HRESULT RenderManager::CreateDeviceAndContext()
{
	SAFE_RELEASE(context_);
	SAFE_RELEASE(device_);
	
	uint32_t createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	std::array<D3D_DRIVER_TYPE, 3> driverTypes = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	std::array<D3D_FEATURE_LEVEL, 7> featureLevels = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	HRESULT hr = S_OK;

	for (uint32_t driverTypeIndex = 0; driverTypeIndex < driverTypes.size(); ++driverTypeIndex)
	{
		driverType_ = driverTypes[driverTypeIndex];
		
		hr = D3D11CreateDevice( 
			nullptr, driverType_, nullptr, createDeviceFlags, &featureLevels[0], static_cast<uint32_t>(std::size(featureLevels)),
			D3D11_SDK_VERSION, &device_, &featureLevel_, &context_
		);

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDevice(
				nullptr, driverType_, nullptr, createDeviceFlags, &featureLevels[1], static_cast<uint32_t>(std::size(featureLevels)) - 1,
				D3D11_SDK_VERSION, &device_, &featureLevel_, &context_
			);
		}

		if (SUCCEEDED(hr)) break;
	}

	return hr;
}

HRESULT RenderManager::CreateSwapChain(HWND windowHandle)
{
	SAFE_RELEASE(swapChain_);

	HRESULT hr = S_OK;

	IDXGIDevice* device = nullptr;
	hr = device_->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&device));
	if (FAILED(hr))
	{
		return hr;
	}

	IDXGIAdapter* adapter = nullptr;
	hr = device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&adapter));
	if (FAILED(hr))
	{
		SAFE_RELEASE(device);
		return hr;
	}

	IDXGIFactory* dxgifactory = nullptr;
	hr = adapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgifactory));
	if (FAILED(hr))
	{
		SAFE_RELEASE(adapter);
		SAFE_RELEASE(device);
		return hr;
	}

	SAFE_RELEASE(adapter);
	SAFE_RELEASE(device);

	int32_t windowWidth = 0;
	int32_t windowHeight = 0;
	renderTargetWindow_->GetSize(windowWidth, windowHeight);

	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	SwapChainDesc.BufferCount = 2;
	SwapChainDesc.BufferDesc.Width = static_cast<uint32_t>(windowWidth);
	SwapChainDesc.BufferDesc.Height = static_cast<uint32_t>(windowHeight);
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.OutputWindow = windowHandle;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.SampleDesc.Quality = 0;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.Windowed = !renderTargetWindow_->IsFullscreen();

	hr = dxgifactory->CreateSwapChain(device_, &SwapChainDesc, &swapChain_);
	hr = dxgifactory->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_ALT_ENTER);

	SAFE_RELEASE(dxgifactory);

	return hr;
}

HRESULT RenderManager::CreateRenderTargetView()
{
	SAFE_RELEASE(renderTargetView_);

	ID3D11Texture2D* backBuffer = nullptr;
	HRESULT hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

	if (SUCCEEDED(hr))
	{
		hr = device_->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView_);
	}

	SAFE_RELEASE(backBuffer);

	return hr;
}

HRESULT RenderManager::CreateDepthStencilView()
{
	SAFE_RELEASE(depthStencilView_);
	SAFE_RELEASE(depthStencilBuffer_);

	int32_t windowWidth = 0, windowHeight = 0;
	renderTargetWindow_->GetSize(windowWidth, windowHeight);

	D3D11_TEXTURE2D_DESC depthStencilBufferDesc = {};
	depthStencilBufferDesc.Width = static_cast<uint32_t>(windowWidth);
	depthStencilBufferDesc.Height = static_cast<uint32_t>(windowHeight);
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.MiscFlags = 0;

	HRESULT hr = device_->CreateTexture2D(&depthStencilBufferDesc, nullptr, &depthStencilBuffer_);

	if (SUCCEEDED(hr))
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		hr = device_->CreateDepthStencilView(depthStencilBuffer_, &depthStencilViewDesc, &depthStencilView_);
	}

	return hr;
}

HRESULT RenderManager::CreateDepthStencilState(ID3D11DepthStencilState** depthStencilState, bool bIsEnableDepth, bool bIsEnableStencil)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc = {};

	depthStencilStateDesc.DepthEnable = static_cast<BOOL>(bIsEnableDepth);
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilStateDesc.StencilEnable = static_cast<BOOL>(bIsEnableStencil);
	depthStencilStateDesc.StencilReadMask = 0xFF;
	depthStencilStateDesc.StencilWriteMask = 0xFF;

	depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	return device_->CreateDepthStencilState(&depthStencilStateDesc, depthStencilState);
}

HRESULT RenderManager::CreateBlendState(ID3D11BlendState** blendState, bool bIsEnable)
{
	D3D11_BLEND_DESC blendStateDesc = {};

	blendStateDesc.RenderTarget[0].BlendEnable = static_cast<BOOL>(bIsEnable);
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return device_->CreateBlendState(&blendStateDesc, blendState);
}

HRESULT RenderManager::CreateRasterizerState(ID3D11RasterizerState** rasterizerState, bool bIsEnableCull, bool bIsEnableFill)
{
	D3D11_RASTERIZER_DESC rasterizerDesc;

	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = (bIsEnableCull ? D3D11_CULL_BACK : D3D11_CULL_NONE);
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = (bIsEnableFill ? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME);
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	return device_->CreateRasterizerState(&rasterizerDesc, rasterizerState);
}
