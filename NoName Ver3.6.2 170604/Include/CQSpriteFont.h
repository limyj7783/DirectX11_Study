#pragma once

class CQSpriteFont : public CQElement2D
{
public:
	CQSpriteFont();
	~CQSpriteFont();

	// CQElement2D을(를) 통해 상속됨
	virtual const Vector2 & GetPosition() const override;
	virtual const Vector2 & GetOrigin() const override;
	virtual const Vector2 & GetScale() const override;
	virtual const Color & GetColor() const override;
	virtual const float & GetAlpha() const override;
	virtual const float & GetRotation() const override;

	virtual void SetPostion(const Vector2 & position) override;
	virtual void SetOrigin(const Vector2 & origin) override;
	virtual void SetScale(const Vector2 & scale) override;
	virtual void SetColor(const Color & color) override;
	virtual void SetAlpha(const float alpha) override;
	virtual void SetRotation(const float rotation) override;

	const wchar_t* GetText() const;
	void SetText(wchar_t* text);

	bool Init(ID3D11Device* pDevice, wchar_t* fileName);
	void ShutDown();
	void DrawString(SpriteBatch* spriteBatch, SpriteFont * spriteFont);
private:
	Vector2 m_Position;
	Vector2 m_Origin;
	Vector2 m_Scale;
	Color m_Color;
	float m_Alpha;
	float m_Rotation;
	wchar_t* m_pText;
	std::unique_ptr<SpriteFont> m_SpriteFont;
};
