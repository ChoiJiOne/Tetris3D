#include <algorithm>
#include <memory>
#include <vector>

#include "AudioManager.h"

// @third party code - BEGIN
#include <miniaudio/miniaudio.h>
// @third party code - END

/**
 * @brief miniaudio 엔진입니다.
 */
static std::unique_ptr<ma_engine> miniAudioEngine = nullptr;

/**
 * @brief 사운드 리소스 오브젝트입니다.
 */
static std::vector<std::unique_ptr<ma_sound>> sounds;

void AudioManager::Setup()
{
	if (bIsSetup_) return;

	miniAudioEngine = std::make_unique<ma_engine>();
	CHECK((ma_engine_init(nullptr, miniAudioEngine.get()) == MA_SUCCESS), "failed to initialize miniaudio...");

	sounds.resize(0);
	bIsSetup_ = true;
}

void AudioManager::Cleanup()
{
	if (!bIsSetup_) return;

	for (auto& sound : sounds)
	{
		ma_sound_uninit(sound.get());
		sound.reset();
	}

	ma_engine_uninit(miniAudioEngine.get());
	miniAudioEngine.reset();
	
	countSoundResource_ = 0;
	bIsSetup_ = false;
}

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}
