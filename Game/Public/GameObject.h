#pragma once

#include "Macro.h"


/**
 * @brief 게임 내 오브젝트입니다.
 */
class GameObject
{
public:
	/**
	 * @brief 게임 내 오브젝트의 기본 생성자입니다.
	 */
	GameObject() = default;


	/**
	 * @brief 게임 내 오브젝트의 생성자입니다.
	 * 
	 * @param updateOrder 게임 오브젝트의 업데이트 순위입니다.
	 * @param bIsActive 게임 오브젝트의 활성화 여부입니다.
	 */
	GameObject(int32_t updateOrder, bool bIsActive)
		: updateOrder_(updateOrder)
		, bIsActive_(bIsActive) {}


	/**
	 * @brief 게임 내 오브젝트의 가상 소멸자입니다.
	 */
	virtual ~GameObject() {}


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(GameObject);


	/**
	 * @brief 게임 오브젝트의 업데이트 순위를 얻습니다.
	 * 
	 * @return 현재 게임 오브젝트의 업데이트 순위를 반환합니다.
	 */
	int32_t GetUpdateOrder() const { return updateOrder_; }


	/**
	 * @brief 게임 오브젝트의 순위를 설정합니다.
	 * 
	 * @param updateOrder 설정할 게임 오브젝트의 순위입니다.
	 */
	void SetUpdateOrder(int32_t updateOrder) { updateOrder_ = updateOrder; }


	/**
	 * @brief 게임 오브젝트가 활성화 되었는지 확인합니다.
	 * 
	 * @return 게임 오브젝트가 활성화 되었다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsActive() const { return bIsActive_; }


	/**
	 * @brief 게임 오브젝트 활성화 여부를 설정합니다.
	 * 
	 * @param bIsActive 게임 오브젝트의 활성화 여부입니다.
	 */
	void SetActive(bool bIsActive) { bIsActive_ = bIsActive; }

	
	/**
	 * @brief 게임 오브젝트를 업데이트합니다.
	 * 
	 * @note 이 함수는 순수 가상 함수로, 하위 클래스에서 반드시 구현해야 합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) = 0;

	
private:
	/**
	 * @brief 게임 오브젝트의 업데이트 순위입니다.
	 * 
	 * @note 낮을 수록 우선 순위가 높습니다.
	 */
	int32_t updateOrder_ = 0;


	/**
	 * @brief 게임 오브젝트의 활성화 여부입니다.
	 * 
	 * @note 비활성화되어 있으면 업데이트되지 않습니다.
	 */
	bool bIsActive_ = false;
};