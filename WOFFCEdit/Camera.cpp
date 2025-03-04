#include "Camera.h"
#include "InputCommands.h"

using namespace DirectX::SimpleMath;

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
	if (!inputCommands.updateBallpointCamera)
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
	else
	{
		MouseDirectionVector = PreviousMousePosition - inputCommands.MousePos;

		Vector2 MouseDirectionVectorNormalized = MouseDirectionVector;
		MouseDirectionVectorNormalized.Normalize();


		//if (MouseDirectionVector.x > 0)
		//{
		//	m_camOrientation.y -= m_camRotRate;
		//}
		//else if (MouseDirectionVector.x < 0)
		//{
		//	m_camOrientation.y += m_camRotRate;
		//}

		//m_camOrientation.z += MouseDirectionVectorNormalized.y * m_camRotRate;
		//m_camOrientation.y += MouseDirectionVectorNormalized.x * m_camRotRate;

		m_camOrientation.z += MouseDirectionVector.y;
		m_camOrientation.y += MouseDirectionVector.x;

		/*if (MouseDirectionVector.y > 0)
		{
			m_camOrientation.z -= MouseDirectionVectorNormalized.y;
		}
		else if (MouseDirectionVector.y < 0)
		{
			m_camOrientation.z += MouseDirectionVectorNormalized.y;
		}*/
	}


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

	//if (m_camOrientation.y > 180)
	//{
	//	m_camOrientation.y = 180;
	//}
	//else if (m_camOrientation.y < -180)
	//{
	//	m_camOrientation.y = -180;
	//}

	//if (pitchDegrees > 180)
	//{
	//	pitchDegrees = 180;
	//}
	//else if (pitchDegrees < 0)
	//{
	//	pitchDegrees = 0;
	//}

	//x = rCosΘCosΦ
	//y = rsinΦ
	//z = rSinΘCosΦ

	//create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = cos(yawRadians) * cos(pitchRadians);
	m_camLookDirection.y = sin(pitchRadians);
	m_camLookDirection.z = sin(yawRadians) * cos(pitchRadians);

	//create right vector from look Direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

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

	PreviousMousePosition = inputCommands.MousePos;
}

Vector2 Camera::CalculateMouseDirection()
{
	return Vector2(0, 0);
}
