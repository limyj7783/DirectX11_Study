#pragma once

struct TextureVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct TextureWorld
{
	XMMATRIX World;
	XMFLOAT4 MeshColor;

	TextureWorld()
	{
		World = XMMatrixIdentity();
		MeshColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}
};

struct TextureView
{
	XMMATRIX View;
};

struct TextureProjection
{
	XMMATRIX Projection;
};

class TextureShader : public CQShader
{
public:
	TextureShader();
	TextureShader(const TextureShader&) = delete;
	virtual ~TextureShader();

	virtual HRESULT CompileVS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device* pDevice) override;
	virtual HRESULT CompilePS(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3D11Device* pDevice) override;
};

static TextureWorld			g_TWorld;
static TextureView			g_TView;
static TextureProjection	g_TProjection;
