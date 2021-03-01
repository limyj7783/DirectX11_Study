#include "../pch.h"
#include "CQCamera.h"

CQCamera::CQCamera()
{
}

CQCamera::~CQCamera()
{
}

XMVECTOR CQCamera::GetPostionVector() const
{
	return XMLoadFloat3( &m_position);
}

XMFLOAT3 CQCamera::GetPosition() const
{
	return m_position;
}

void CQCamera::SetPosition(float px, float py, float pz)
{
	m_position = XMFLOAT3(px, py, pz);
}

XMVECTOR CQCamera::GetRightVector() const
{
	return XMLoadFloat3( &m_right );
}

XMVECTOR CQCamera::GetUpVector() const
{
	return XMLoadFloat3( &m_up );
}

XMVECTOR CQCamera::GetLookVector() const
{
	return XMLoadFloat3( &m_look);
}

XMFLOAT3 CQCamera::GetRight() const
{
	return m_right;
}

XMFLOAT3 CQCamera::GetUp() const
{
	return m_up;
}

XMFLOAT3 CQCamera::GetLook() const
{
	return m_look;
}

void CQCamera::SetProjMatrix(float FOV, float Aspect, float Near, float Far)
{
	m_Fov = FOV;
	m_Aspect = Aspect;
	m_NearZ = Near;
	m_FarZ = Far;
	XMMATRIX P = XMMatrixPerspectiveFovLH(FOV, Aspect, Near, Far);
	XMStoreFloat4x4(&m_proj, P);
}

void CQCamera::SetViewMatrix(FXMVECTOR Pos, FXMVECTOR Target, FXMVECTOR WorldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(Target, Pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(WorldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&m_position, Pos);
	XMStoreFloat3(&m_look, L);
	XMStoreFloat3(&m_right, R);
	XMStoreFloat3(&m_up, U);
}

void CQCamera::SetViewMatrix(const XMFLOAT3 & Pos, const XMFLOAT3 & Target, const XMFLOAT3 & Up)
{
	XMVECTOR P = XMLoadFloat3(&Pos);
	XMVECTOR T = XMLoadFloat3(&Target);
	XMVECTOR U = XMLoadFloat3(&Up);

	SetViewMatrix(P, T, U);
}

XMMATRIX CQCamera::GetView() const
{
	return XMLoadFloat4x4(&m_view);
}

XMMATRIX CQCamera::GetProj() const
{
	return XMLoadFloat4x4(&m_proj);
}

XMMATRIX CQCamera::GetViewProj() const
{
	return XMMatrixMultiply(GetView(), GetProj());
}

void CQCamera::Walk(float speed)
{
	XMVECTOR s = XMVectorReplicate(speed);
	XMVECTOR l = XMLoadFloat3(&m_look);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, l, p));
}

void CQCamera::Strafe(float speed)
{
	XMVECTOR s = XMVectorReplicate(speed);
	XMVECTOR r = XMLoadFloat3(&m_right);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, r, p));
}

void CQCamera::RotateX(float angle)
{
}

void CQCamera::RotateY(float angle)
{
	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), R));
	XMStoreFloat3(&m_up   , XMVector3TransformNormal(XMLoadFloat3(&m_up)   , R));
	XMStoreFloat3(&m_look , XMVector3TransformNormal(XMLoadFloat3(&m_look) , R));
}

void CQCamera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&m_right);
	XMVECTOR U = XMLoadFloat3(&m_up);
	XMVECTOR L = XMLoadFloat3(&m_look);
	XMVECTOR P = XMLoadFloat3(&m_position);

	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));
	R = XMVector3Cross(U, L);

	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&m_right, R);
	XMStoreFloat3(&m_up, U);
	XMStoreFloat3(&m_look, L);

	m_view(0, 0) = m_right.x;
	m_view(1, 0) = m_right.y;
	m_view(2, 0) = m_right.z;
	m_view(3, 0) = x;

	m_view(0, 1) = m_up.x;
	m_view(1, 1) = m_up.y;
	m_view(2, 1) = m_up.z;
	m_view(3, 1) = y;

	m_view(0, 2) = m_look.x;
	m_view(1, 2) = m_look.y;
	m_view(2, 2) = m_look.z;
	m_view(3, 2) = z;

	m_view(0, 3) = 0.0f;
	m_view(1, 3) = 0.0f;
	m_view(2, 3) = 0.0f;
	m_view(3, 3) = 1.0f;
}

bool CQCamera::Init(int width, int height, ID3D11Device* pDevice) 
{
	m_world = XMMatrixIdentity();

	Pos		= { 0.0f, 0.0f, -5.0f };
	Target	= { 0.0f, 0.0f, 1.0f };
	Up		= { 0.0f, 1.0f, 0.0f };

	SetViewMatrix(Pos, Target, Up);

	SetProjMatrix(XM_PIDIV2, width / float(height), 0.01f, 100.0f);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	bd.ByteWidth = sizeof(TextureWorld);
	pDevice->CreateBuffer(&bd, nullptr, &m_pTWorld);
	pDevice->CreateBuffer(&bd, nullptr, &g_pTWorld);

	bd.ByteWidth = sizeof(TextureView);
	pDevice->CreateBuffer(&bd, nullptr, &m_pTView);

	bd.ByteWidth = sizeof(TextureProjection);
	pDevice->CreateBuffer(&bd, nullptr, &m_pTProjection);

	return true;
}

void CQCamera::Shutdown()
{
	ReleaseCOM(m_pTProjection);
	ReleaseCOM(m_pTView);
	ReleaseCOM(m_pTWorld);
	ReleaseCOM(g_pTWorld);
}

void CQCamera::Render(ID3D11DeviceContext* pImmediateContext )
{
	/*ConstantBuffer cb;
	cb.World = XMMatrixTranspose(m_world);
	cb.View = XMMatrixTranspose(GetView());
	cb.Proj = XMMatrixTranspose(GetProj());

	pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &cb, 0,0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	*/

	g_TView.View = XMMatrixTranspose(GetView());
	pImmediateContext->UpdateSubresource(m_pTView, 0, nullptr, &g_TView, 0, 0);

	g_TProjection.Projection = XMMatrixTranspose(GetProj());
	pImmediateContext->UpdateSubresource(m_pTProjection, 0, nullptr, &g_TProjection, 0, 0);
	
	g_TWorld.World = XMMatrixTranspose(m_world);
	pImmediateContext->UpdateSubresource(m_pTWorld, 0, nullptr, &g_TWorld, 0, 0);

	pImmediateContext->PSSetConstantBuffers(2, 1, &m_pTWorld);
	pImmediateContext->VSSetConstantBuffers(2, 1, &m_pTWorld);
	pImmediateContext->VSSetConstantBuffers(0, 1, &m_pTView);
	pImmediateContext->VSSetConstantBuffers(1, 1, &m_pTProjection);

}

void CQCamera::Update()
{
	if (KEY_MGR->IsKeyDown(VK_UP))
		Walk(0.002f);

	if (KEY_MGR->IsKeyDown(VK_DOWN))
		Walk(-0.002f);

	if (KEY_MGR->IsKeyDown(VK_LEFT))
		Strafe(-0.002f);

	if (KEY_MGR->IsKeyDown(VK_RIGHT) )
		Strafe(0.002f);

	if (KEY_MGR->IsKeyDown('D'))
		RotateY( XMConvertToRadians( 0.052f ) );
	if (KEY_MGR->IsKeyDown('A'))
		RotateY(XMConvertToRadians( -0.052f));

	if (KEY_MGR->IsKeyDown('W'))
		m_position.y += 0.005f;
	if (KEY_MGR->IsKeyDown('S'))
		m_position.y -= 0.005f;

	if(XBOX_MGR->GetState().Gamepad.wButtons == XINPUT_GAMEPAD_A)
		Walk(0.002f);

	UpdateViewMatrix();
}

