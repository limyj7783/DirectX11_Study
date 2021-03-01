#pragma once

const float SCREEN_NEAR = 0.01f;
const float SCREEN_FAR  = 1000.0f;

class CQGraphic
{

public:
	CQGraphic();
	CQGraphic(const CQGraphic&) = delete;
	~CQGraphic();

	bool Init(int Width, int Height, HWND hWnd);
	void Shutdown();
	bool Update( );

private:
	bool Render();

public:
	DX*				m_pDX;
	CQCamera*		m_pCQCamera;
	ColorShader*	m_pColorShader;
	TextureShader*	m_pTextureShader;

	CQTextureBox* m_pTextureBox;

	std::unique_ptr<SpriteBatch> m_SpriteBatch;
	std::unique_ptr<SpriteFont> m_SpriteFont;

	CQSpriteBatch* m_pCQSpriteBatch;
	CQSpriteFont* m_pCQSpriteFont;
};

