#pragma once

#include <functional>

#include "Scene.h"


/**
 * @brief 게임 종료 씬입니다.
 */
class EndScene : public Scene
{
public:
	/**
	 * @brief 게임 종료 씬의 생성자입니다.
	 *
	 * @param updateOrder 게임 종료 씬의 업데이트 순위입니다.
	 * @param bIsActive 게임 종료 씬의 활성화 여부입니다.
	 */
	EndScene(int32_t updateOrder, bool bIsActive)
		: Scene(updateOrder, bIsActive) {}


	/**
	 * @brief 게임 종료 씬의 가상 소멸자입니다.
	 */
	virtual ~EndScene() {}


	/**
	 * @brief 복사 생성자 및 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(EndScene);


	/**
	 * @brief 게임 종료 씬을 업데이트합니다.
	 *
	 * @param deltaSeconds 초단위 델타 시간값입니다.
	 */
	virtual void Tick(float deltaSeconds) override;


	/**
	 * @brief 게임 종료 씬에 진입합니다.
	 */
	virtual void Entry() override;


	/**
	 * @brief 게임 종료 씬으로부터 나갑니다.
	 */
	virtual void Leave() override;


	/**
	 * @brief 게임 루프를 종료하는 이벤트를 설정합니다.
	 *
	 * @param quitEvent 설정할 게임 루프를 종료하는 이벤트입니다.
	 */
	void SetQuitEvent(const std::function<void()>& quitEvent) { quitEvent_ = quitEvent; }


private:
	/**
	 * @brief 시작 씬 내의 UI 업데이트 순위입니다.
	 */
	int32_t uiUpdateOrder_ = 0;


	/**
	 * @brief 게임 종료 이벤트입니다.
	 */
	std::function<void()> quitEvent_ = nullptr;
};