#pragma once

class CQColorBox
{
public:
	CQColorBox();
	CQColorBox(const CQColorBox&) = delete;
	~CQColorBox();

	bool Init(ID3D11Device* pDevice, WCHAR* dds = nullptr);
	void Shutdown();
	void Render(ID3D11DeviceContext* pImmediateContext);

	UINT GetIndexCount() const;

private:
	bool InitBuffers(ID3D11Device* pDevice, WCHAR* dds = nullptr);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* pImmediateContext);

private:
	ID3D11Buffer*	m_pVertexBuffer;
	ID3D11Buffer*	m_pIndexBuffer;

	CQTexture*		m_pTexture;

	UINT m_VertexCount;
	UINT m_IndexCount;
	UINT m_Stride;
	UINT m_Offset;
};