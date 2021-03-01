#include "../pch.h"
#include "CQTextureBox.h"

CQTextureBox::CQTextureBox()
{
	m_pTexture = nullptr;
}

CQTextureBox::~CQTextureBox()
{
}

bool CQTextureBox::Init(ID3D11Device * pDevice, WCHAR * dds)
{
	return InitBuffers(pDevice, dds);
}

void CQTextureBox::Shutdown()
{
	ShutdownBuffers();
}

void CQTextureBox::Render(ID3D11DeviceContext * pImmediateContext)
{
	RenderBuffers(pImmediateContext);
}

UINT CQTextureBox::GetIndexCount() const
{
	return m_IndexCount;
}

bool CQTextureBox::InitBuffers(ID3D11Device * pDevice, WCHAR * dds)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));

	TextureVertex Vertices[] =
	{
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2( 1.0f, 0.0f) },
		{ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT2( 0.0f, 0.0f) },
		{ XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT2( 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2( 1.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2( 0.0f, 0.0f) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT2( 1.0f, 0.0f) },
		{ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT2( 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2( 0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2( 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2( 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2( 1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2( 0.0f, 0.0f) },

		{ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT2( 1.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT2( 0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT2( 0.0f, 0.0f) },
		{ XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT2( 1.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT2( 0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, -1.0f, -1.0f), XMFLOAT2( 1.0f, 1.0f) },
		{ XMFLOAT3( 1.0f,  1.0f, -1.0f), XMFLOAT2( 1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT2( 0.0f, 0.0f) },

		{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT2( 1.0f, 1.0f) },
		{ XMFLOAT3( 1.0f, -1.0f,  1.0f), XMFLOAT2( 0.0f, 1.0f) },
		{ XMFLOAT3( 1.0f,  1.0f,  1.0f), XMFLOAT2( 0.0f, 0.0f) },
		{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT2( 1.0f, 0.0f) },
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(TextureVertex) * ARRAYSIZE(Vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	data.pSysMem = Vertices;
	hr = pDevice->CreateBuffer(&bd, &data, &m_pVertexBuffer);
	if (FAILED(hr))
		return false;

	WORD Indices[] =
	{
		3,1,0,
		2,1,3,

		6,4,5,
		7,4,6,

		11,9,8,
		10,9,11,

		14,12,13,
		15,12,14,

		19,17,16,
		18,17,19,

		22,20,21,
		23,20,22,
	};
	bd.ByteWidth = sizeof(WORD) * ARRAYSIZE(Indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	data.pSysMem = Indices;
	hr = pDevice->CreateBuffer(&bd, &data, &m_pIndexBuffer);
	if (FAILED(hr))
		return false;

	m_Stride = sizeof(TextureVertex);
	m_Offset = 0;
	m_IndexCount = ARRAYSIZE(Indices);

	// Texture Init
	if (dds)
	{
		m_pTexture = new CQTexture();
		m_pTexture->Init(pDevice, dds);
	}

	return true;
}

void CQTextureBox::ShutdownBuffers()
{
	if (m_pTexture)
	{
		m_pTexture->Shutdown();
		delete m_pTexture;
	}
	ReleaseCOM(m_pIndexBuffer);
	ReleaseCOM(m_pVertexBuffer);
}

void CQTextureBox::RenderBuffers(ID3D11DeviceContext * pImmediateContext)
{
	pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_Stride, &m_Offset);
	pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	if (m_pTexture)
	{
		ID3D11ShaderResourceView* pSRV = m_pTexture->GetTexture();
		pImmediateContext->PSSetShaderResources(0, 1, &pSRV);
	}

	pImmediateContext->DrawIndexed(m_IndexCount, 0, 0);
}
