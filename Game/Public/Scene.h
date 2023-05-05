#pragma once

#include "GameObject.h"


/**
 * @brief 게임 내 장면에 배치될 오브젝트를 관리합니다.
 */
class Scene : public GameObject
{
public:
	/**
	 * @brief 게임 씬의 생성자입니다.
	 * 
	 * @param updateOrder 게임 씬의 업데이트 순위입니다.
	 * @param bIsActive 게임 씬의 활성화 여부입니다.
	 */
	Scene(int32_t updateOrder, bool bIsActive)
		: GameObject(updateOrder, bIsActive) {}


	/**
	 * @brief 게임 씬의 가상 소멸자입니다.
	 */
	virtual ~Scene() {}


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(Scene);


	/**
	 * @brief 게임 씬을 업데이트합니다.
	 * 
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) = 0;


	/**
	 * @brief 다음 씬을 연결합니다.
	 * 
	 * @param nextScene 연결할 다음 씬입니다.
	 */
	void LinkNextScene(Scene* nextScene) { nextScene_ = nextScene; }
	

	/**
	 * @brief 현재 씬에 진입합니다.
	 */
	virtual void Entry() = 0;


	/**
	 * @brief 현재 씬으로부터 나갑니다.
	 */
	virtual void Leave() = 0;
	

protected:
	/**
	 * @brief 씬 전환이 감지되었는지 확인합니다.
	 */
	bool bIsDetectSwitch_ = false;


	/**
	 * @brief 현재 게임 씬에 연결된 다음 씬입니다.
	 */
	Scene* nextScene_ = nullptr;
};