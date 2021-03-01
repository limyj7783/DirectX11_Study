#include "../pch.h"
#include "CQColorBox.h"

CQColorBox::CQColorBox()
{
	m_pTexture = nullptr;
}

CQColorBox::~CQColorBox()
{
}

bool CQColorBox::Init(ID3D11Device * pDevice, WCHAR * dds)
{
	return InitBuffers(pDevice, dds);
}

void CQColorBox::Shutdown()
{
	ShutdownBuffers();
}

void CQColorBox::Render(ID3D11DeviceContext * pImmediateContext)
{
	RenderBuffers(pImmediateContext);
}

UINT CQColorBox::GetIndexCount() const
{
	return m_IndexCount;
}

bool CQColorBox::InitBuffers(ID3D11Device * pDevice, WCHAR * dds)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));

	ColorVertex Vertices[] =
	{
	{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
	};

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ColorVertex) * ARRAYSIZE(Vertices);
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

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};
	bd.ByteWidth = sizeof(WORD) * ARRAYSIZE(Indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	data.pSysMem = Indices;
	hr = pDevice->CreateBuffer(&bd, &data, &m_pIndexBuffer);
	if (FAILED(hr))
		return false;

	m_Stride = sizeof(ColorVertex);
	m_Offset = 0;
	m_IndexCount = sizeof(WORD) * ARRAYSIZE(Indices);

	// Texture Init
	if (dds)
	{
		m_pTexture = new CQTexture();
		m_pTexture->Init(pDevice, dds);
	}

	return true;
}

void CQColorBox::ShutdownBuffers()
{
	if (m_pTexture)
	{
		m_pTexture->Shutdown();
		delete m_pTexture;
	}
	ReleaseCOM(m_pIndexBuffer);
	ReleaseCOM(m_pVertexBuffer);
}

void CQColorBox::RenderBuffers(ID3D11DeviceContext * pImmediateContext)
{
	pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_Stride, &m_Offset);
	pImmediateContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pImmediateContext->DrawIndexed(m_IndexCount, 0, 0);
}
