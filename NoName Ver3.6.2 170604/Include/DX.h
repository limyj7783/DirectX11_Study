#pragma once

class DX
{
public :
	DX();
	DX(const DX&) = delete;
	~DX();

	bool Init(int Width, int Height, HWND hWnd, float Near, float Far);
	void Shutdown();

	void BeginScene(float R, float G, float B, float A);
	void BeginScene(XMVECTORF32 C);
	void EndScene();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

private :
	// DX 11.1 or later
	ID3D11Device1*				m_pDevice1;
	ID3D11DeviceContext1*		m_pImmediateContext1;
	IDXGISwapChain1*			m_pSwapChain1;

	// DX 11
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pImmediateContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pRTV;
	ID3D11Texture2D*			m_pDepthStencil;
	ID3D11DepthStencilView*		m_pDSV;
	ID3D11SamplerState*			m_pSamplerState;

	D3D_DRIVER_TYPE				m_DriverType;
	D3D_FEATURE_LEVEL			m_FeatureLevel;
};