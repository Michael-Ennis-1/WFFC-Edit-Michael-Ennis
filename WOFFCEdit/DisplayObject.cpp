#include "DisplayObject.h"
#include "SimpleMath.h"
#include <DirectXMath.h>

DisplayObject::DisplayObject()
{
	m_model = NULL;
	m_texture_diffuse = NULL;
	m_orientation.x = 0.0f;
	m_orientation.y = 0.0f;
	m_orientation.z = 0.0f;
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;
	m_scale.x = 0.0f;
	m_scale.y = 0.0f;
	m_scale.z = 0.0f;
	m_render = true;
	m_wireframe = false;

	m_light_type =0;
	m_light_diffuse_r = 0.0f;	m_light_diffuse_g = 0.0f;	m_light_diffuse_b = 0.0f;
	m_light_specular_r = 0.0f;	m_light_specular_g = 0.0f;	m_light_specular_b = 0.0f;
	m_light_spot_cutoff = 0.0f;
	m_light_constant = 0.0f;
	m_light_linear = 0.0f;
	m_light_quadratic = 0.0f;
}


DisplayObject::~DisplayObject()
{
//	delete m_texture_diffuse;
}

DirectX::XMMATRIX DisplayObject::GetObjectMatrix()
{
	// Initialize translation, scale and rotation vectors to create object-local matrix
	const DirectX::XMVECTOR translation = { m_position.x, m_position.y, m_position.z };
	const DirectX::XMVECTOR scale = { m_scale.x, m_scale.y, m_scale.z };
	const DirectX::XMVECTOR rotation = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_orientation.y * 3.1415 / 180,
																							m_orientation.x * 3.1415 / 180,
																							m_orientation.z * 3.1415 / 180);

	return DirectX::XMMatrixTransformation(DirectX::XMVECTORF32{ 0, 0, 0, 0 }, DirectX::SimpleMath::Quaternion::Identity, scale,
											DirectX::XMVECTORF32{ 0, 0, 0, 0 }, rotation, translation);
}
