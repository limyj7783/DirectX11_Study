#pragma once

class CQKeyMgr
{
	// friend 
	friend CQSingleton<CQKeyMgr>;

public :
	bool	IsKeyDown(int Key);
	bool	KeyDown(int Key);
	bool	KeyUp(int Key);
	bool	IsToggle(int Key);

	void	SetFocus(bool Focus);
	bool	GetFocus();

private :
	bool m_Focus;
	bool m_DownKey[KEY_MAX];
	bool m_UpKey[KEY_MAX];
	bool m_ToggleKey[KEY_MAX];

private :
	CQKeyMgr();
	~CQKeyMgr();

	CQKeyMgr(const CQKeyMgr&) = delete;
	CQKeyMgr& operator = (const CQKeyMgr&) = delete;
};