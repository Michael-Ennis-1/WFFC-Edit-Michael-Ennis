#include "Camera.h"
#include "InputCommands.h"

using namespace DirectX::SimpleMath;

Camera::Camera()
{
	m_slowspeed = 1.0f;
	m_movespeed = 3.0f;
	m_sprintspeed = 9.0f;

	m_camRotRate = 1.0;
	m_camRotSensitivity = 0.5f;

	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;
	m_camOrientation.y = 0;
	m_camOrientation.z = 0;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;
}

void Camera::Update(InputCommands inputCommands, float deltaTime)
{
	if (inputCommands.rightMouseDown)
	{
		MouseDirectionVector = PreviousMousePosition - inputCommands.MousePos;

		// Explicitly don't normalize mouse direction vector, so further we drag faster we rotate.
		// Allows for fine tuned control over rotation
		m_camOrientation.z += MouseDirectionVector.y * m_camRotSensitivity;
		m_camOrientation.y += MouseDirectionVector.x * m_camRotSensitivity;
	}
	else
	{
		if (inputCommands.rotRight)
		{
			m_camOrientation.y -= m_camRotRate;
		}
		if (inputCommands.rotLeft)
		{
			m_camOrientation.y += m_camRotRate;
		}
	}

	// Clamp camera orientation to within 90 degrees up or down
	if (m_camOrientation.z > 90)
	{
		m_camOrientation.z = 90;
	}
	else if (m_camOrientation.z < -90)
	{
		m_camOrientation.z = -90;
	}

	// Pi in radians
	float Pi = 3.1415 / 180;

	float yawRadians = -m_camOrientation.y * Pi;
	float pitchRadians = m_camOrientation.z * Pi;

	// Parametric equation of a sphere used
	// x = rCosΘCosΦ
	// y = rsinΦ
	// z = rSinΘCosΦ

	// Create look direction from Polar and Azimuthal angles in m_camOrientation
	m_camLookDirection.x = cos(yawRadians) * cos(pitchRadians);
	m_camLookDirection.y = sin(pitchRadians);
	m_camLookDirection.z = sin(yawRadians) * cos(pitchRadians);

	// Create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	// Increase or decrease move speed based on keys pressed
	float movespeed = m_movespeed;
	if (inputCommands.shift)
	{
		movespeed = m_sprintspeed;
	}
	else if (inputCommands.ctrl)
	{
		movespeed = m_slowspeed;
	}

	//  Move camera based on input, also limit it based on deltatime
	if (inputCommands.forward)
	{
		m_camPosition += m_camLookDirection * movespeed * deltaTime;
	}
	if (inputCommands.back)
	{
		m_camPosition -= m_camLookDirection * movespeed * deltaTime;
	}
	if (inputCommands.right)
	{
		m_camPosition += m_camRight * movespeed * deltaTime;
	}
	if (inputCommands.left)
	{
		m_camPosition -= m_camRight * movespeed * deltaTime;
	}

	// Update lookat point for camera direction
	m_camLookAt = m_camPosition + m_camLookDirection;

	// Cache previous frame's mouse position, to determine how far mouse was dragged this frame
	PreviousMousePosition = inputCommands.MousePos;

	// Calculate view and projection matrices
	m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);
}

void Camera::UpdateProjectionView(float aspectRatio, float fovAngleY)
{
	m_projection = Matrix::CreatePerspectiveFieldOfView(fovAngleY, aspectRatio, 0.01f, 1000.0f);
}
