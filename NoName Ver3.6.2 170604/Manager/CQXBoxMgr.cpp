#include "../pch.h"
#include "CQXBoxMgr.h"

CQXBoxMgr::CQXBoxMgr()
{
	ZeroMemory(&state, sizeof(state));
	ZeroMemory(&keyStroke, sizeof(keyStroke));
	ZeroMemory(&vibration, sizeof(vibration));
}

CQXBoxMgr::~CQXBoxMgr()
{
}

void CQXBoxMgr::Init()
{
	DWORD result;
	for (int i = 0; i < XUSER_MAX_COUNT; i++) {
		result = XInputGetState(i, &state);
		if (result == ERROR_SUCCESS) {
			controllerID = i;
		}
	}
}

XINPUT_STATE CQXBoxMgr::GetState()
{
	XInputGetState(controllerID, &state);
	return state;
}

XINPUT_KEYSTROKE CQXBoxMgr::GetKeyStroke()
{
	keyStroke.UserIndex = controllerID;
	XInputGetKeystroke(controllerID, 0, &keyStroke);
	return keyStroke;
}

XINPUT_VIBRATION CQXBoxMgr::Vibrate(float left, float right)
{
	vibration.wLeftMotorSpeed = (int)(left*65535.0f);
	vibration.wRightMotorSpeed = (int)(right*65535.0f);

	XInputSetState(controllerID, &vibration);

	return vibration;
}

int CQXBoxMgr::GetControllerID() const
{
	return controllerID;
}