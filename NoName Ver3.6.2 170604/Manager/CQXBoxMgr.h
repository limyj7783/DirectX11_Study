#pragma once

class CQXBoxMgr
{
	friend CQSingleton<CQXBoxMgr>;

public:
	void Init();
	XINPUT_STATE GetState();
	XINPUT_KEYSTROKE GetKeyStroke();
	XINPUT_VIBRATION Vibrate(float left, float right);
	int GetControllerID()const;

private:
	XINPUT_STATE state;
	XINPUT_KEYSTROKE keyStroke;
	XINPUT_VIBRATION vibration;
	int controllerID;

private:
	CQXBoxMgr();
	~CQXBoxMgr();
	CQXBoxMgr(const CQXBoxMgr&) = delete;
	CQXBoxMgr& operator = (const CQXBoxMgr&) = delete;
};
