#pragma once
#include <d3d12.h>
#include <SimpleMath.h>
struct InputCommands;

class Camera
{
public:
	Camera();

	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;
	float m_camRotRate;
	float m_movespeed;

	void Update(InputCommands inputCommands);
};