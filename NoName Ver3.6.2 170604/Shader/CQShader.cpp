#include "../pch.h"
#include "CQShader.h"

CQShader::CQShader()
{
	m_pVS = nullptr;
	m_pIL = nullptr;
	m_pPS = nullptr;
}

CQShader::~CQShader()
{
	ReleaseCOM(m_pPS);
	ReleaseCOM(m_pIL);
	ReleaseCOM(m_pVS);
}

void CQShader::SetShader(ID3D11DeviceContext * pImmediateContext)
{
	pImmediateContext->IASetInputLayout(m_pIL);
	pImmediateContext->VSSetShader(m_pVS, nullptr, 0);
	pImmediateContext->PSSetShader(m_pPS, nullptr, 0);
}

ID3D11VertexShader * CQShader::GetVS()
{
	return m_pVS;
}

ID3D11InputLayout * CQShader::GetIL()
{
	return m_pIL;
}

ID3D11PixelShader * CQShader::GetPS()
{
	return m_pPS;
}

HRESULT CQShader::CompileShader(WCHAR * szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob ** ppBlobOut)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined(_DEBUG) || defined( DEBUG )
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr;

	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel, dwShaderFlags,
		0, ppBlobOut, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			OutputDebugString(reinterpret_cast<LPCWSTR>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}

		return hr;
	}

	if (pErrorBlob)
		pErrorBlob->Release();

	return S_OK;
}
