#include "pch.h"
#include "Include\NoName.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR CmdLine, int CmdShow)
{
	NoName* pNoName = nullptr;

	pNoName = new NoName(hInstance);
	if (pNoName->Init())
		pNoName->Run();

	pNoName->Shutdown();
	delete pNoName;

	return 0;
}