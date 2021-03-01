#pragma once

struct ColorVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

class ColorShader : public CQShader
{
public :
	ColorShader();
	ColorShader(const ColorShader&) = delete;
	virtual ~ColorShader();

	virtual HRESULT CompileVS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device* pDevice) override;
	virtual HRESULT CompilePS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device* pDevice) override;
};