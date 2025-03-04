#pragma once
#include <d3d12.h>
#include <SimpleMath.h>
struct InputCommands;

class Camera
{
public:
	Camera();

	void Update(InputCommands inputCommands);

	DirectX::SimpleMath::Vector2 CalculateMouseDirection();

	DirectX::SimpleMath::Vector2 PreviousMousePosition = { 0, 0 };
	DirectX::SimpleMath::Vector2 MouseDirectionVector = { 0, 0 };

	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;
	float m_camRotRate;
	float m_movespeed;

	float m_pitchDegrees;
	float m_yawDegrees;
};