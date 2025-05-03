#pragma once
#include <d3d12.h>
#include <SimpleMath.h>
struct InputCommands;

class Camera
{
public:
	Camera();

	// Updates camera variables
	void Update(InputCommands inputCommands, float deltaTime);

	// Dynamically update projection based on new aspect ration and FOV angle
	void UpdateProjectionView(float aspectRatio, float fovAngleY);

	// Stores previous mouse pos on screen and mouse direction based on previous and current mouse pos
	DirectX::SimpleMath::Vector2 PreviousMousePosition = { 0, 0 };
	DirectX::SimpleMath::Vector2 MouseDirectionVector = { 0, 0 };

	// Stores camera specific vectors
	DirectX::SimpleMath::Vector3		m_camPosition;
	DirectX::SimpleMath::Vector3		m_camOrientation;
	DirectX::SimpleMath::Vector3		m_camLookAt;
	DirectX::SimpleMath::Vector3		m_camLookDirection;
	DirectX::SimpleMath::Vector3		m_camRight;

	// Stores camera view and projection matrices
	DirectX::SimpleMath::Matrix         m_view;
	DirectX::SimpleMath::Matrix         m_projection;

	// Used for QE simple rotation
	float m_camRotRate;

	// Used for camera sensitivity based on mouse drag
	float m_camRotSensitivity;

	// Used to control camera move speed
	float m_movespeed;
};