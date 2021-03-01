#pragma once


// MANAGER HEADER
#include "../Manager/CQSingleton.h"
#include "../Manager/CQKeyMgr.h"
#include "../Manager/CQXBoxMgr.h"

// ENGINE HEADER
#include "../Include/CQTimer.h"










#define KEY_MGR		CQSingleton<CQKeyMgr>::GetInstance()
#define XBOX_MGR	CQSingleton<CQXBoxMgr>::GetInstance()