#include "..\pch.h"
#include "CQSpriteFont.h"

CQSpriteFont::CQSpriteFont()
{
}

CQSpriteFont::~CQSpriteFont()
{
}

const Vector2 & CQSpriteFont::GetPosition() const
{
	return m_Position;
}

const Vector2 & CQSpriteFont::GetOrigin() const
{
	return m_Origin;
}

const Vector2 & CQSpriteFont::GetScale() const
{
	return m_Scale;
}

const Color & CQSpriteFont::GetColor() const
{
	return m_Color;
}

const float & CQSpriteFont::GetAlpha() const
{
	return m_Alpha;
}

const float & CQSpriteFont::GetRotation() const
{
	return m_Rotation;
}

void CQSpriteFont::SetPostion(const Vector2 & position)
{
	m_Position = position;
}

void CQSpriteFont::SetOrigin(const Vector2 & origin)
{
	m_Origin = origin;
}

void CQSpriteFont::SetScale(const Vector2 & scale)
{
	m_Scale = scale;
}

void CQSpriteFont::SetColor(const Color & color)
{
	m_Color = color;
}

void CQSpriteFont::SetAlpha(const float alpha)
{
	m_Alpha = alpha;
}

void CQSpriteFont::SetRotation(const float rotation)
{
	m_Rotation = rotation;
}

const wchar_t * CQSpriteFont::GetText() const
{
	return m_pText;
}

void CQSpriteFont::SetText(wchar_t * text)
{
	m_pText = text;
}

bool CQSpriteFont::Init(ID3D11Device* pDevice, wchar_t* fileName)
{
	m_Position = Vector2(0, 0);
	m_SpriteFont.reset(new SpriteFont(pDevice, fileName));
	return true;
}

void CQSpriteFont::ShutDown()
{
	delete m_pText;
	m_pText = nullptr;
}

void CQSpriteFont::DrawString(SpriteBatch* spriteBatch, SpriteFont * spriteFont)
{
	spriteFont->DrawString(spriteBatch, m_pText, m_Position, m_Color, m_Rotation, m_Origin, m_Scale, SpriteEffects::SpriteEffects_None, 0.0f);
}
