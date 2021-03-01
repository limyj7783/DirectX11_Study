#pragma once

// Custom define
#include "pch\CQdefine.h"

#include <Windows.h>
#include <Windowsx.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>

// DirectX Header
#include <d3d11_1.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <dwrite.h>
#include "Include\DDSTextureLoader.h"
#include <xaudio2.h>
#include <Xinput.h>

//DirectX ToolKit Header
#include "DirectXTK-master\Inc\SpriteBatch.h"
#include "DirectXTK-master\Inc\SimpleMath.h"
#include "DirectXTK-master\Inc\SpriteFont.h"

using namespace DirectX;


// Shader
#include "pch\CQShader.h"


#include "pch\CQGame.h"

#include "pch\CQEngine.h"

#include "pch\CQFrameWork.h"




// NoName 
#include "Include\NoName.h"

static CQCamera* g_Camera = nullptr;
static ID3D11Buffer* g_pTWorld = nullptr;