#pragma once

#include <functional>

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


	/**
	 * @brief 게임 루프를 종료하는 이벤트를 설정합니다.
	 * 
	 * @param quitEvent 설정할 게임 루프를 종료하는 이벤트입니다.
	 */
	void SetQuitEvent(const std::function<void()>& quitEvent) { quitEvent_ = quitEvent; }


private:
	/**
	 * @brief 타이틀 텍스트의 색상을 업데이트합니다.
	 */
	void UpdateTitleTextColor();


private:
	/**
	 * @brief 시작 씬 내의 UI 업데이트 순위입니다.
	 */
	int32_t uiUpdateOrder_ = 0;


	/**
	 * @brief 게임 종료 이벤트입니다.
	 */
	std::function<void()> quitEvent_ = nullptr;


	/**
	 * @brief 시작 씬의 누적 시간값입니다.
	 */
	float accumulateTime_ = 0.0f;


	/**
	 * @brief 테트로미노의 최대 속도 값입니다.
	 */
	float maxTetrominoSpeed_ = 0.1f;


	/**
	 * @brief 테트로미노의 최소 속도 값입니다.
	 */
	float minTetrominoSpeed_ = 1.5f;


	/**
	 * @brief 테트로미노의 속도값입니다.
	 */
	float tetrominoSpeed_ = 1.0f;
};