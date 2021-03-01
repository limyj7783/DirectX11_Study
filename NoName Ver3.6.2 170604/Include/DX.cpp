#include "../pch.h"
#include "DX.h"

DX::DX()
{
	m_pDevice1				= nullptr;
	m_pImmediateContext1	= nullptr;
	m_pSwapChain1			= nullptr;

	m_pSwapChain			= nullptr;
	m_pDevice				= nullptr;
	m_pImmediateContext		= nullptr;
	m_pRTV					= nullptr;
	m_pDepthStencil			= nullptr;
	m_pDSV					= nullptr;
	m_pSamplerState			= nullptr;
}

DX::~DX()
{
}

bool DX::Init(int Width, int Height, HWND hWnd, float Near, float Far)
{
	HRESULT hr;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, m_DriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &m_pDevice, &m_FeatureLevel, &m_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, m_DriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &m_pDevice, &m_FeatureLevel, &m_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return false;

	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return false;

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = m_pDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pDevice1));
		if (SUCCEEDED(hr))
		{
			(void)m_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pImmediateContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = Width;
		sd.Height = Height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(m_pDevice, hWnd, &sd, nullptr, nullptr, &m_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = m_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = Width;
		sd.BufferDesc.Height = Height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain);
	}
	dxgiFactory->Release();
	if (FAILED(hr))
		return false;

	// Create RenderTargetView
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return false;

	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRTV);
	pBackBuffer->Release();
	if (FAILED(hr))
		return false;

	// Create Depth Stencil Texture
	D3D11_TEXTURE2D_DESC DepthStencilTexture;
	ZeroMemory(&DepthStencilTexture, sizeof(DepthStencilTexture));
	DepthStencilTexture.Width = Width;
	DepthStencilTexture.Height = Height;
	DepthStencilTexture.MipLevels = 1;
	DepthStencilTexture.ArraySize = 1;
	DepthStencilTexture.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthStencilTexture.SampleDesc.Count = 1;
	DepthStencilTexture.SampleDesc.Quality = 0;
	DepthStencilTexture.Usage = D3D11_USAGE_DEFAULT;
	DepthStencilTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DepthStencilTexture.CPUAccessFlags = 0;
	DepthStencilTexture.MiscFlags = 0;
	hr = m_pDevice->CreateTexture2D(&DepthStencilTexture, nullptr, &m_pDepthStencil);
	if (FAILED(hr))
		return false;

	// Create Depth Stencil View
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencil, &descDSV, &m_pDSV);
	if (FAILED(hr))
		return false;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV, m_pDSV);

	// Create Sampler State
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = m_pDevice->CreateSamplerState(&SamDesc, &m_pSamplerState);
	if (FAILED(hr))
		return false;

	// Setup Viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)Width;
	vp.Height = (FLOAT)Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0;
	vp.TopLeftY = 0.0;
	m_pImmediateContext->RSSetViewports(1, &vp);

	return true;
}

void DX::Shutdown()
{
	if (m_pImmediateContext)
		m_pImmediateContext->ClearState();

	ReleaseCOM(m_pSamplerState);
	ReleaseCOM(m_pDSV);
	ReleaseCOM(m_pDepthStencil);
	ReleaseCOM(m_pRTV);
	ReleaseCOM(m_pImmediateContext);
	ReleaseCOM(m_pImmediateContext1);
	ReleaseCOM(m_pDevice);
	ReleaseCOM(m_pDevice1);
	ReleaseCOM(m_pSwapChain);
	ReleaseCOM(m_pSwapChain1);
}

void DX::BeginScene(float R, float G, float B, float A)
{
	float Color[4] = { R, G, B, A };

	m_pImmediateContext->ClearRenderTargetView(m_pRTV, Color);
	m_pImmediateContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerState);
}

void DX::BeginScene(XMVECTORF32 C)
{
	m_pImmediateContext->ClearRenderTargetView(m_pRTV, C);
	m_pImmediateContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamplerState);
}

void DX::EndScene()
{
	m_pSwapChain->Present(0, 0);
}

ID3D11Device * DX::GetDevice() const
{
	return m_pDevice;
}

ID3D11DeviceContext * DX::GetDeviceContext() const
{
	return m_pImmediateContext;
}
