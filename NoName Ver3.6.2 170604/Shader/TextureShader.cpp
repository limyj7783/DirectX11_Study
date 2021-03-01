#include "../pch.h"
#include "TextureShader.h"

D3D11_INPUT_ELEMENT_DESC TextureLayout[] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

TextureShader::TextureShader()
{
}

TextureShader::~TextureShader()
{
}

HRESULT TextureShader::CompileVS(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device * pDevice)
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = nullptr;

	hr = CompileShader(szFileName, szEntryPoint, szShaderModel, &pBlob);
	if (FAILED(hr))
		return hr;

	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pVS);
	if (FAILED(hr))
	{
		pBlob->Release();
		return hr;
	}

	hr = pDevice->CreateInputLayout(TextureLayout, ARRAYSIZE(TextureLayout), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pIL);
	pBlob->Release();

	return hr;
}

HRESULT TextureShader::CompilePS(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device * pDevice)
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = nullptr;

	hr = CompileShader(szFileName, szEntryPoint, szShaderModel, &pBlob);
	if (FAILED(hr))
		return hr;

	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPS);
	pBlob->Release();

	return hr;
}
