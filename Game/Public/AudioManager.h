#pragma once

#include "Macro.h"


/**
 * @brief 게임 내 오디오를 관리합니다.
 * 
 * @note 이 클래스는 싱글턴 클래스로 헤더만 추가하면 바로 사용할 수 있습니다.
 */
class AudioManager
{
public:
	/**
	 * @brief 복사 생성자와 대입 연산자를 명시적으로 삭제합니다.
	 */
	DISALLOW_COPY_AND_ASSIGN(AudioManager);


	/**
	 * @brief AudioManager의 인스턴스를 얻습니다.
	 * 
	 * @return AudioManager의 인스턴스 참조자를 반환합니다.
	 */
	static AudioManager& Get()
	{
		static AudioManager instance;
		return instance;
	}


	/**
	 * @brief AudioManager를 명시적으로 초기화합니다.
	 * 
	 * @note 초기화 했거나 초기화 이후 Cleanup이 호출되지 않았다면 아무 동작도 수행하지 않습니다.
	 */
	void Setup();


	/**
	 * @brief AudioManager의 내부 요소를 명시적으로 정리합니다.
	 * 
	 * @note 초기화한 적이 없거나 Cleanup 호출 이후에 Setup을 호출하지 않았다면 아무 동작도 수행하지 않습니다.
	 */
	void Cleanup();


private:
	/**
	 * @brief 게임 내 오디오를 관리하는 클래스의 생성자입니다.
	 */
	AudioManager();


	/**
	 * @brief 게임 내 오디오를 관리하는 클래스의 가상 소멸자입니다.
	 */
	virtual ~AudioManager();


private:
	/**
	 * @brief 초기화된 적이 있는지 확인합니다.
	 */
	bool bIsSetup_ = false;


	/**
	 * @brief 생성된 사운드 리소스의 수입니다.
	 */
	int32_t countSoundResource_ = 0;
};