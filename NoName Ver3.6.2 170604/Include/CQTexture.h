#pragma once

#include "DDSTextureLoader.h"

class CQTexture
{
public :
	CQTexture();
	~CQTexture();

	bool Init(ID3D11Device* pDevice, WCHAR* dds);
	void Shutdown();

	ID3D11ShaderResourceView*	GetTexture();

private :
	ID3D11ShaderResourceView*	m_pTextureRV;
};
