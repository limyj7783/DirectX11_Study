#include "../pch.h"
#include "CQGraphic.h"

CQGraphic::CQGraphic()
{
	m_pDX = nullptr;
	m_pColorShader = nullptr;
	m_pCQCamera = nullptr;
}

CQGraphic::~CQGraphic()
{
}

bool CQGraphic::Init(int Width, int Height, HWND hWnd)
{
	m_pDX = new DX();
	if (!m_pDX->Init(Width, Height, hWnd, SCREEN_NEAR, SCREEN_FAR))
		return false;

	m_pTextureBox = new CQTextureBox();
	if (!m_pTextureBox->Init(m_pDX->GetDevice(), L"Resources/Texture/wood.dds"))
		return false;

	m_pColorShader = new ColorShader();
	m_pColorShader->CompileVS(L"Shader/ColorShader.fx", "ColorVertexShader", "vs_4_0", m_pDX->GetDevice());
	m_pColorShader->CompilePS(L"Shader/ColorShader.fx", "ColorPixelShader", "ps_4_0", m_pDX->GetDevice());

	m_pTextureShader = new TextureShader();
	m_pTextureShader->CompileVS(L"Shader/TextureShader.fx", "TextureVertexShader", "vs_4_0", m_pDX->GetDevice());
	m_pTextureShader->CompilePS(L"Shader/TextureShader.fx", "TexturePixelShader", "ps_4_0", m_pDX->GetDevice());
	
	m_pCQCamera = new CQCamera();
	if (!m_pCQCamera->Init(Width, Height, m_pDX->GetDevice()))
		return false;

	g_Camera = m_pCQCamera;

	m_SpriteBatch.reset(new SpriteBatch(m_pDX->GetDeviceContext()));
	m_pCQSpriteBatch = new CQSpriteBatch();
	if (!m_pCQSpriteBatch->Init(m_pDX->GetDevice(), L"Resources\\Texture\\UITest.dds"))
		return false;
	
	m_SpriteFont.reset(new SpriteFont(m_pDX->GetDevice(), L"Resources\\Font\\ClearGothic.spritefont"));

	return true;
}

void CQGraphic::Shutdown()
{
	if (m_pTextureShader)
	{
		delete m_pTextureShader;
		m_pTextureShader = nullptr;
	}

	if (m_pColorShader)
	{
		delete m_pColorShader;
		m_pColorShader = nullptr;
	}

	if (m_pTextureBox)
	{
		m_pTextureBox->Shutdown();
		delete m_pTextureBox;
		m_pTextureBox = nullptr;
	}

	if (m_pDX)
	{
		m_pDX->Shutdown();
		delete m_pDX;
		m_pDX = nullptr;
	}

	if (m_pCQCamera) {
		m_pCQCamera->Shutdown();
		delete m_pCQCamera;
		m_pCQCamera = nullptr;
	}
	
	if (m_pCQSpriteBatch) {
		m_pCQSpriteBatch->ShutDown();
		delete m_pCQSpriteBatch;
		m_pCQSpriteBatch = nullptr;
	}
}

bool CQGraphic::Update( )
{
	m_pCQCamera->Update();
	return Render();
}

bool CQGraphic::Render()
{
	// m_pDX->BeginScene(0.0f, 0.75f, 0.75f, 1.0f);

		m_pDX->BeginScene(Colors::MidnightBlue);

	// m_pColorShader->SetShader(m_pDX->GetDeviceContext());

	m_pTextureShader->SetShader(m_pDX->GetDeviceContext());
	m_pCQCamera->Render(m_pDX->GetDeviceContext());
	// m_pCQModel->Render(m_pDX->GetDeviceContext());
	m_pTextureBox->Render(m_pDX->GetDeviceContext());
	
	m_SpriteBatch->Begin();
	m_pCQSpriteBatch->SetPostion(SimpleMath::Vector2(100, 100));
	m_pCQSpriteBatch->Draw(m_SpriteBatch.get());
	
	m_SpriteFont->DrawString(m_SpriteBatch.get(), L"Hello World", SimpleMath::Vector2(200, 200), SimpleMath::Color(1.0f,1.0f,0.0f));
	m_SpriteBatch->End();

	m_pDX->EndScene();

	return true;
}
