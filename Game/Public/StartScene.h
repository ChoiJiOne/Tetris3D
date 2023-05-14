#pragma once

#include "Scene.h"


/**
 * @brief 게임 시작 씬입니다.
 */
class StartScene : public Scene
{
public:
	/**
	 * @brief 게임 시작 씬의 생성자입니다.
	 * 
	 * @param updateOrder 게임 시작 씬의 업데이트 순위입니다.
	 * @param bIsActive 게임 시작 씬의 활성화 여부입니다.
	 */
	StartScene(int32_t updateOrder, bool bIsActive)
		: Scene(updateOrder, bIsActive) {}


	/**
	 * @brief 게임 시작 씬의 가상 소멸자입니다.
	 */
	virtual ~StartScene() {}


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(StartScene);


	/**
	 * @brief 게임 시작 씬을 업데이트합니다.
	 *
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 게임 시작 씬에 진입합니다.
	 */
	virtual void Entry() override;


	/**
	 * @brief 게임 시작 씬으로부터 나갑니다.
	 */
	virtual void Leave() override;


private:
	/**
	 * @brief 시작 씬 내의 UI 업데이트 순위입니다.
	 */
	int32_t uiUpdateOrder_ = 0;
};