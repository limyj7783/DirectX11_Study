#pragma once

class CQCamera
{
public:
	CQCamera();
	CQCamera(const CQCamera&) = delete;
	~CQCamera();

	XMVECTOR GetPostionVector() const;
	XMFLOAT3 GetPosition() const;
	void SetPosition(float px, float py, float pz);

	// Get Camera basis vectores
	XMVECTOR GetRightVector() const;
	XMVECTOR GetUpVector() const;
	XMVECTOR GetLookVector() const;

	XMFLOAT3 GetRight() const;
	XMFLOAT3 GetUp() const;
	XMFLOAT3 GetLook() const;

	// Set Projection
	void SetProjMatrix(float FOV, float Aspect, float Near, float Far);

	// Set View
	void SetViewMatrix(FXMVECTOR Pos, FXMVECTOR Target, FXMVECTOR WorldUp);
	void SetViewMatrix(const XMFLOAT3& Pos, const XMFLOAT3& Target, const XMFLOAT3& Up);

	// Get Matrix
	XMMATRIX GetView() const;
	XMMATRIX GetProj() const;
	XMMATRIX GetViewProj() const;

	// Camera Move
	void Walk(float speed);
	void Strafe(float speed);

	// Camera Rotate
	void RotateX(float angle);
	void RotateY(float angle);

	// View Matrix Update
	void UpdateViewMatrix();

	bool Init(int width, int height, ID3D11Device* pDevice);
	void Render(ID3D11DeviceContext* pImmediateContext);
	void Update();
	void Shutdown();

private:

	XMFLOAT3 m_position; //카메라 위치
	XMFLOAT3 m_look;	 //보는 방향
	XMFLOAT3 m_up;		 //up벡터
	XMFLOAT3 m_right;

	float m_rotateSpeed; //회전속도
	float m_wheelSensitive; //휠 민감도
	float m_moveSpeed;	//이동속도

	// Frustum Properties
	float m_Fov;
	float m_NearZ;
	float m_FarZ;
	float m_Aspect;

	XMMATRIX m_world; 
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;
	XMMATRIX m_viewProj;

	ID3D11Buffer* m_pTWorld;
	ID3D11Buffer* m_pTView;
	ID3D11Buffer* m_pTProjection;

	XMFLOAT3 Pos;
	XMFLOAT3 Target;
	XMFLOAT3 Up;
};