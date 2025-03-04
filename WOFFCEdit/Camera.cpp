#include "Camera.h"
#include "InputCommands.h"

Camera::Camera()
{
	m_movespeed = 0.3f;
	m_camRotRate = 3.0;

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

void Camera::Update(InputCommands inputCommands)
{
	if (inputCommands.rotRight)
	{
		m_camOrientation.y -= m_camRotRate;
	}
	if (inputCommands.rotLeft)
	{
		m_camOrientation.y += m_camRotRate;
	}

	//create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = sin((m_camOrientation.y) * 3.1415 / 180);
	m_camLookDirection.z = cos((m_camOrientation.y) * 3.1415 / 180);
	m_camLookDirection.Normalize();

	//create right vector from look Direction
	m_camLookDirection.Cross(DirectX::SimpleMath::Vector3::UnitY, m_camRight);

	//process input and update stuff
	if (inputCommands.forward)
	{
		m_camPosition += m_camLookDirection * m_movespeed;
	}
	if (inputCommands.back)
	{
		m_camPosition -= m_camLookDirection * m_movespeed;
	}
	if (inputCommands.right)
	{
		m_camPosition += m_camRight * m_movespeed;
	}
	if (inputCommands.left)
	{
		m_camPosition -= m_camRight * m_movespeed;
	}

	//update lookat point
	m_camLookAt = m_camPosition + m_camLookDirection;
}
