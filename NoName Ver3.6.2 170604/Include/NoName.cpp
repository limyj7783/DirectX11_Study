#include "../pch.h"
#include "NoName.h"

int WINDOW_WIDTH = 1024;
int WINDOW_HEIGHT = 768;

NoName::NoName(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_Width		= WINDOW_WIDTH;
	m_Height	= WINDOW_HEIGHT;
}

NoName::~NoName()
{
}

bool NoName::Init()
{
	InitWindow();
	if (!m_hWnd)
		return false;

	m_pCQGraphic = new CQGraphic();
	if( !m_pCQGraphic->Init(m_Width, m_Height, m_hWnd) )
		return false;

	m_pCQBgm = new CQBGM();
	if (!m_pCQBgm->Init())
		return false;
//	m_pCQBgm->Play();

	return true;
}

void NoName::Shutdown()
{
	m_pCQGraphic->Shutdown();

	if (m_pCQBgm) {
		delete m_pCQBgm;
		m_pCQBgm = nullptr;
	}
	ShutdownWindow();
}

void NoName::Run()
{
	MSG msg = { 0 };
	m_Timer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Timer Update
			 m_Timer.Tick();
			// Print FPS
			CalculateFrame();

			// Game Update
			if (!Update()) 
				break;
		}
	}
}

bool NoName::Update()
{
	if (KEY_MGR->KeyDown(VK_ESCAPE))
		return false;

	m_pCQGraphic->Update();


	return true;
}

LRESULT NoName::NoNameHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETFOCUS : 
		KEY_MGR->SetFocus(true);
		return 0;
	case WM_KILLFOCUS :
		KEY_MGR->SetFocus(true);
		return 0;

	case WM_KEYDOWN:
		return 0;
	case WM_KEYUP:
		return 0;


	case WM_MOUSEMOVE:
		return 0;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		return 0;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		return 0;


	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

void NoName::InitWindow()
{
	g_pNoName = this;
	m_Name = L"Project NoName";

	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_Name;

	if (!RegisterClass(&wc))
		return;

	int X = (GetSystemMetrics(SM_CXSCREEN) - m_Width) / 2;
	int Y = (GetSystemMetrics(SM_CYSCREEN) - m_Height) / 2;

	RECT rc = { 0, 0, m_Width, m_Height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	m_hWnd = CreateWindow(
		m_Name,
		m_Name,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		X,
		Y,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		m_hInstance,
		nullptr
	);

	if (!m_hWnd)
		return;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	ShowCursor(true);
}

void NoName::ShutdownWindow()
{
	DestroyWindow(m_hWnd);
	UnregisterClass(m_Name, m_hInstance);
	g_pNoName = nullptr;
}

void NoName::CalculateFrame()
{
	static int _FrameCount = 0;
	static float _TimeElapsed = 0.0f;
	static std::wstring Name(m_Name);

	++_FrameCount;

	if ((m_Timer.GetTotalTime() - _TimeElapsed) >= 1.0f)
	{
		float fps = static_cast<float>(_FrameCount);
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		std::wstring Caption = Name + L"    FPS: " + std::to_wstring(_FrameCount);
		const wchar_t* FPS = Caption.c_str();
		outs.precision(6);
		outs << Caption;
		SetWindowText(m_hWnd, FPS);


		_FrameCount = 0;
		_TimeElapsed += 1.0f;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	default:
		return g_pNoName->NoNameHandler(hWnd, uMsg, wParam, lParam);
	}
}
