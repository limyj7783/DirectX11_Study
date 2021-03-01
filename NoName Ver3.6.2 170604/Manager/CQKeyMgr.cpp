#include "../pch.h"
#include "CQKeyMgr.h"

bool CQKeyMgr::IsKeyDown(int Key)
{
	bool a = m_Focus && (GetAsyncKeyState(Key) & 0x8000);

	if (a)
	{
		int b;
		b = 1;
	}

	return (m_Focus && (GetAsyncKeyState(Key) & 0x8000));
}

bool CQKeyMgr::KeyDown(int Key)
{
	if (m_Focus && (GetAsyncKeyState(Key) & 0x8000))
	{
		if (!m_DownKey[Key])
		{
			m_DownKey[Key] = true;
			return true;
		}
	}
	else
	{
		m_DownKey[Key] = false;
	}

	return false;
}

bool CQKeyMgr::KeyUp(int Key)
{
	if (m_Focus && (GetAsyncKeyState(Key) &0x8000))
	{
		m_UpKey[Key] = true;
	}
	else
	{
		if (m_UpKey[Key])
		{
			m_UpKey[Key] = false;
			return true;
		}
	}

	return false;
}

bool CQKeyMgr::IsToggle(int Key)
{
	if (IsKeyDown(Key))
		m_ToggleKey[Key] = !m_ToggleKey[Key];

	return m_ToggleKey[Key];
}

void CQKeyMgr::SetFocus(bool Focus)
{
	m_Focus = Focus;
}

bool CQKeyMgr::GetFocus()
{
	return m_Focus;
}

CQKeyMgr::CQKeyMgr()
{
	m_Focus = false;
	ZeroMemory(m_DownKey, sizeof(m_DownKey));
	ZeroMemory(m_UpKey, sizeof(m_UpKey));
	ZeroMemory(m_ToggleKey, sizeof(m_ToggleKey));
}

CQKeyMgr::~CQKeyMgr()
{
}
