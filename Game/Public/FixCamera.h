#pragma once

#include <directxmath.h>

#include "GameObject.h"


/**
 * @brief 게임 내 고정 카메라 오브젝트입니다.
 */
class FixCamera : public GameObject
{
public:
	/**
	 * @brief 게임 내 고정 카메라 오브젝트의 생성자입니다.
	 * 
	 * @param updateOrder 고정 카메라의 업데이트 순위입니다.
	 * @param bIsActive 고정 카메라의 활성화 여부입니다.
	 * @param eyePosition 월드 상의 카메라 위치입니다.
	 * @param focusPosition 월드 상의 카메라가 바라보고 있는 위치입니다.
	 * @param upDirection 월드 상 카메라의 위쪽 방향입니다.
	 * @param fovAngleY 원근 투영 행렬의 하향식 시야각(라디안)입니다.
	 * @param aspectRatio 뷰 공간 X:Y의 가로 세로 비율입니다.
	 * @param nearZ 가까운 클리핑 평면까지의 거리입니다. 기본 값은 0.01입니다.
	 * @param farZ 원거리 클리핑 평면까지의 거리입니다. 기본 값은 1000.0입니다.
	 */
	FixCamera(
		int32_t updateOrder, 
		bool bIsActive,
		const DirectX::XMFLOAT3& eyePosition,
		const DirectX::XMFLOAT3& focusPosition,
		const DirectX::XMFLOAT3& upDirection,
		float fovAngleY,
		float aspectRatio,
		float nearZ = 0.01f,
		float farZ = 1000.0f
	);


	/**
	 * @brief 게임 내 고정 카메라 오브젝트의 가상 소멸자입니다.
	 */
	virtual ~FixCamera() {}


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(FixCamera);

	
	/**
	 * @brief 고정 카메라 오브젝트를 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 뷰 공간 X:Y의 가로 세로 비율을 설정합니다.
	 * 
	 * @param aspectRatio 설정할 가로 세로 비율값입니다.
	 */
	void SetAspectRatio(float aspectRatio);


	/**
	 * @brief 고정 카메라의 시야 행렬을 얻습니다.
	 * 
	 * @return 시야 행렬을 반환합니다.
	 */
	DirectX::XMMATRIX GetViewMatrix() const { return viewMatrix_; }


	/**
	 * @brief 고정 카메라의 원근 투영 행렬을 얻습니다.
	 * 
	 * @return 원근 투영 행렬을 반환합니다.
	 */
	DirectX::XMMATRIX GetProjectionMatrix() const { return projectionMatrix_; }


private:
	/**
	 * @brief 고정 카메라의 월드 상 위치입니다.
	 */
	DirectX::XMVECTOR eyePosition_;


	/**
	 * @brief 고정 카메라가 바라보고 있는 위치입니다.
	 */
	DirectX::XMVECTOR focusPosition_;


	/**
	 * @brief 고정 카메라의 위쪽 방향입니다.
	 */
	DirectX::XMVECTOR upDirection_;


	/**
	 * @brief 원근 투영 행렬의 하향식 시야각(라디안)입니다.
	 */
	float fovAngleY_;


	/**
	 * @brief 뷰 공간 X:Y의 가로 세로 비율입니다.
	 */
	float aspectRatio_;


	/**
	 * @brief 가까운 클리핑 평면까지의 거리입니다
	 *
	 * @note 0보다 커야 합니다.
	 */
	float nearZ_;


	/**
	 * @brief 원거리 클리핑 평면까지의 거리입니다. 
	 * 
	 * @note 0보다 커야 합니다.
	 */
	float farZ_;


	/**
	 * @brief 시야 행렬입니다.
	 */
	DirectX::XMMATRIX viewMatrix_;


	/**
	 * @brief 원근 투영 행렬입니다.
	 */
	DirectX::XMMATRIX projectionMatrix_;
};