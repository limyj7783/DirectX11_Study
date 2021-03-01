#pragma once


class CQShader
{
public:
	CQShader();
	CQShader(const CQShader&) = delete;
	virtual ~CQShader();

	virtual HRESULT CompileVS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device* pDevice) = 0;
	virtual HRESULT CompilePS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device* pDevice) = 0;
	void SetShader(ID3D11DeviceContext* pImmediateContext);

	ID3D11VertexShader* GetVS();
	ID3D11InputLayout*  GetIL();
	ID3D11PixelShader*	GetPS();

protected:
	HRESULT CompileShader(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

protected:
	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader*	m_pPS;
	ID3D11InputLayout*	m_pIL;
};

// VS : VertexShader
// PS : PixelShader
// IL : InputLayout