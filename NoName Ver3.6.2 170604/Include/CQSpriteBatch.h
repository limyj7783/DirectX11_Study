#pragma once

class CQSpriteBatch : public CQElement2D
{
public:
	CQSpriteBatch();
	~CQSpriteBatch();

	// CQElement2D을(를) 통해 상속됨
	//Getter
	virtual const Vector2 & GetPosition() const override;
	virtual const Vector2 & GetOrigin() const override;
	virtual const Vector2 & GetScale() const override;
	virtual const Color & GetColor() const override;
	virtual const float & GetAlpha() const override;
	virtual const float & GetRotation() const override;

	//Setter
	virtual void SetPostion(const Vector2 & position) override;
	virtual void SetOrigin(const Vector2 & origin) override;
	virtual void SetScale(const Vector2 & scale) override;
	virtual void SetColor(const Color & color) override;
	virtual void SetAlpha(const float alpha) override;
	virtual void SetRotation(const float rotation) override;

	bool Init(ID3D11Device* pDevice, const wchar_t* fileName);
	void ShutDown();
	void Draw(SpriteBatch * spriteBatch);
private:
	bool Load(ID3D11Device* pDevice, const wchar_t* fileName);
	void GetTextureDim(ID3D11Resource* resource, UINT* width, UINT* height);

protected:
	Vector2 m_Position;
	Vector2 m_Origin;
	Vector2 m_Scale;
	Color m_Color;
	float m_Alpha;
	float m_Rotation;
	
	RECT m_SourceRect;
	UINT m_Width;
	UINT m_Height;

	ID3D11Resource* m_pResource;
	ID3D11ShaderResourceView* m_pTexture;
};