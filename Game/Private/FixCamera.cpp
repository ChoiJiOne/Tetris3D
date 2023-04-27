#include "FixCamera.h"

FixCamera::FixCamera(
	const DirectX::XMFLOAT3& eyePosition, 
	const DirectX::XMFLOAT3& focusPosition, 
	const DirectX::XMFLOAT3& upDirection, 
	float fovAngleY, 
	float aspectRatio, 
	float nearZ, 
	float farZ
) : GameObject(1, true)
, fovAngleY_(fovAngleY)
, aspectRatio_(aspectRatio)
, nearZ_(nearZ)
, farZ_(farZ)
{
	eyePosition_ = DirectX::XMVectorSet(eyePosition.x, eyePosition.y, eyePosition.z, 0.0f);
	focusPosition_ = DirectX::XMVectorSet(focusPosition.x, focusPosition.y, focusPosition.z, 0.0f);
	upDirection_ = DirectX::XMVectorSet(upDirection.x, upDirection.y, upDirection.z, 0.0f);

	viewMatrix_ = DirectX::XMMatrixLookAtLH(eyePosition_, focusPosition_, upDirection_);
	projectionMatrix_ = DirectX::XMMatrixPerspectiveFovLH(fovAngleY_, aspectRatio_, nearZ_, farZ_);
}

void FixCamera::Tick(float deltaSeconds)
{
}

void FixCamera::SetAspectRatio(float aspectRatio)
{
	aspectRatio_ = aspectRatio;
	projectionMatrix_ = DirectX::XMMatrixPerspectiveFovLH(fovAngleY_, aspectRatio_, nearZ_, farZ_);
}