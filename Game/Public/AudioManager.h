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


	/**
	 * @brief 사운드 리소스를 생성합니다.
	 *
	 * @param path 사운드 리소스의 경로입니다.
	 *
	 * @return 생성된 사운드 리소스의 아이디를 반환합니다. 생성에 실패하면 -1을 반환합니다.
	 */
	int32_t CreateSound(const std::string& path);


	/**
	 * @brief 사운드의 크기를 설정합니다.
	 *
	 * @param soundID 크기를 설정할 사운드의 아이디입니다.
	 * @param volume 사운드의 크기입니다. 범위는 0.0 ~ 1.0 입니다.
	 */
	void SetSoundVolume(int32_t soundID, float volume);


	/**
	 * @brief 현재 사운드의 크기를 얻습니다.
	 *
	 * @param soundID 소리 크기를 얻을 사운드의 아이디입니다.
	 *
	 * @return 현재 사운드의 크기를 얻습니다.
	 */
	float GetSoundVolume(int32_t soundID);


	/**
	 * @brief 사운드의 반복 여부를 설정합니다.
	 *
	 * @param soundID 반복 여부를 설정할 사운드의 아이디입니다.
	 * @param bIsLoop 사운드 반복 여부입니다.
	 */
	void SetSoundLooping(int32_t soundID, bool bIsLoop);


	/**
	 * @brief 사운드의 반복 여부를 얻습니다.
	 *
	 * @param soundID 반복 여부를 확인할 사운드의 아이디입니다.
	 *
	 * @return 사운드가 반복한다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	bool IsSoundLooping(int32_t soundID);


	/**
	 * @brief 사운드를 플레이합니다.
	 *
	 * @note 이전에 중지한 적이 있다면 중지한 시점부터 플레이됩니다.
	 *
	 * @param soundID 플레이할 사운드의 아이디입니다.
	 */
	void PlaySound(int32_t soundID);


	/**
	 * @brief 사운드가 플레이중인지 확인합니다.
	 *
	 * @param soundID 플레이 중인지 확인할 사운드 아이디입니다.
	 *
	 * @return 사운드가 플레이 중이라면 true, 그렇지 않다면 false를 반환합니다.
	 */
	bool IsPlayingSound(int32_t soundID);


	/**
	 * @brief 사운드 플레이가 끝났는지 확인합니다.
	 *
	 * @param soundID 플레이가 끝났는지 확인할 사운드 아이디입니다.
	 *
	 * @return 사운드 플레이가 끝났다면 true, 그렇지 않다면 false를 반환합니다.
	 */
	bool IsDoneSound(int32_t soundID);


	/**
	 * @brief 사운드 플레이를 중지합니다.
	 *
	 * @param soundID 플레이를 중지할 사운드 아이디입니다.
	 */
	void StopSound(int32_t soundID);


	/**
	 * @brief 사운드 플레이가 중지 되었는지 확인합니다.
	 *
	 * @param soundID 플레이가 중지 되었는지 확인할 사운드 아이디입니다.
	 *
	 * @return 사운드 플레이가 중지 되었다면 true, 그렇지 않으면 false를 반환합니다.
	 */
	bool IsStoppingSound(int32_t soundID);


	/**
	 * @brief 사운드를 초기화합니다.
	 *
	 * @param soundID 초기화할 사운드의 아이디입니다.
	 */
	void ResetSound(int32_t soundID);


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