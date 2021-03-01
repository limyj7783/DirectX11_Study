#pragma once

class NoName
{
public:
	NoName(HINSTANCE hInstance);
	NoName(const NoName&) = delete;
	~NoName();

	bool Init();
	void Shutdown();
	void Run();

	LRESULT CALLBACK NoNameHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	bool Update();
	void InitWindow();
	void ShutdownWindow();
	void CalculateFrame();

private:
	LPCWSTR		m_Name;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
	int			m_Width;
	int			m_Height;

	CQGraphic*  m_pCQGraphic;
	CQTimer		m_Timer;
	CQBGM*      m_pCQBgm;
};

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static NoName* g_pNoName = nullptr;