#include "../pch.h"
#include "CQTexture.h"

CQTexture::CQTexture()
{
	m_pTextureRV = nullptr;
}

CQTexture::~CQTexture()
{
	Shutdown();
}

bool CQTexture::Init(ID3D11Device * pDevice, WCHAR * dds)
{
	HRESULT hr;

	hr = CreateDDSTextureFromFile(pDevice, dds, nullptr, &m_pTextureRV);

	return (hr == S_OK);
}

void CQTexture::Shutdown()
{
	if( m_pTextureRV )	
		ReleaseCOM(m_pTextureRV);	
}

ID3D11ShaderResourceView * CQTexture::GetTexture()
{
	return m_pTextureRV;
}
