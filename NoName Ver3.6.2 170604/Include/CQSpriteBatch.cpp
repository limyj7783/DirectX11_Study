#include "..\pch.h"
#include "CQSpriteBatch.h"

CQSpriteBatch::CQSpriteBatch()
{
}

CQSpriteBatch::~CQSpriteBatch()
{
}

const Vector2 & CQSpriteBatch::GetPosition() const
{
	return m_Position;
}

const Vector2 & CQSpriteBatch::GetOrigin() const
{
	return m_Origin;
}

const Vector2 & CQSpriteBatch::GetScale() const
{
	return m_Scale;
}

const Color & CQSpriteBatch::GetColor() const
{
	return m_Color;
}

const float & CQSpriteBatch::GetAlpha() const
{
	return m_Alpha;
}

const float & CQSpriteBatch::GetRotation() const
{
	return m_Rotation;
}

void CQSpriteBatch::SetPostion(const Vector2 & position)
{
	m_Position = position;
}

void CQSpriteBatch::SetOrigin(const Vector2 & origin)
{
	m_Origin = origin;
}

void CQSpriteBatch::SetScale(const Vector2 & scale)
{
	m_Scale = scale;
}

void CQSpriteBatch::SetColor(const Color & color)
{
	m_Color = color;
}

void CQSpriteBatch::SetAlpha(const float alpha)
{
	m_Alpha = alpha;
}

void CQSpriteBatch::SetRotation(const float rotation)
{
	m_Rotation = rotation;
}

void CQSpriteBatch::Draw(SpriteBatch * spriteBatch)
{
	spriteBatch->Draw(m_pTexture, m_Position, &m_SourceRect, m_Color, m_Rotation, m_Origin, m_Scale, SpriteEffects::SpriteEffects_None, 0.0f);
}

bool CQSpriteBatch::Init(ID3D11Device* pDevice, const wchar_t* fileName)
{
	m_Position = Vector2(0, 0);
	m_Alpha = 1.0f;
	m_Rotation = 0.0f;
	m_Scale = Vector2(1, 1);
	m_Color = Color(Colors::White.v);
	m_pResource = nullptr;
	if (!Load(pDevice, fileName))
		return false;
	
	return true;
}

void CQSpriteBatch::ShutDown()
{
	ReleaseCOM(m_pResource);
	ReleaseCOM(m_pTexture);
}

bool CQSpriteBatch::Load(ID3D11Device * pDevice, const wchar_t * fileName)
{
	HRESULT hr;
	hr = CreateDDSTextureFromFile(pDevice, fileName, &m_pResource, &m_pTexture);
	if (FAILED(hr))
		return false;

	GetTextureDim(m_pResource, &m_Width, &m_Height);
	
	m_Origin = Vector2(m_Width / 2.0f, m_Height / 2.0f);
	
	m_SourceRect.left = 0;
	m_SourceRect.top = 0;
	m_SourceRect.right = m_Width;
	m_SourceRect.bottom = m_Height;

	return true;
}

void CQSpriteBatch::GetTextureDim(ID3D11Resource * resource, UINT * width, UINT * height)
{
	D3D11_RESOURCE_DIMENSION dim;
	resource->GetType(&dim);

	switch (dim)
	{
		case D3D11_RESOURCE_DIMENSION_TEXTURE2D:
			{
				auto texture = reinterpret_cast<ID3D11Texture2D*>(resource);
				D3D11_TEXTURE2D_DESC desc;
				texture->GetDesc(&desc);
				if (width)
					*width = desc.Width;
				if (height)
					*height = desc.Height;
			}
			break;
		default:
			{
				if (width)
					*width = 0;
				if (height)
					*height = 0;
			}
			break;
	}
}
