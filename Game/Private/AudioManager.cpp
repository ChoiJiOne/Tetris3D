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

int32_t AudioManager::CreateSound(const std::string& path)
{
	std::unique_ptr<ma_sound> sound = std::make_unique<ma_sound>();

	if (ma_sound_init_from_file(miniAudioEngine.get(), path.c_str(), 0, nullptr, nullptr, sound.get()) != MA_SUCCESS)
	{
		return -1;
	}

	sounds.push_back(std::move(sound));
	return countSoundResource_++;
}

void AudioManager::SetSoundVolume(int32_t soundID, float volume)
{
	if (soundID == -1) return;

	volume = std::clamp<float>(volume, 0.0f, 1.0f);
	ma_sound_set_volume(sounds[soundID].get(), volume);
}

float AudioManager::GetSoundVolume(int32_t soundID)
{
	if (soundID == -1) return 0.0f;

	return ma_sound_get_volume(sounds[soundID].get());
}

void AudioManager::SetSoundLooping(int32_t soundID, bool bIsLoop)
{
	if (soundID == -1) return;

	ma_sound_set_looping(sounds[soundID].get(), static_cast<ma_bool32>(bIsLoop));
}

bool AudioManager::IsSoundLooping(int32_t soundID)
{
	if (soundID == -1) return false;

	return ma_sound_is_looping(sounds[soundID].get()) == MA_TRUE;
}

void AudioManager::PlaySound(int32_t soundID)
{
	if (soundID == -1) return;

	ma_sound_start(sounds[soundID].get());
}

bool AudioManager::IsPlayingSound(int32_t soundID)
{
	if (soundID == -1) return false;

	return ma_sound_is_playing(sounds[soundID].get()) == MA_TRUE;
}

bool AudioManager::IsDoneSound(int32_t soundID)
{
	if (soundID == -1) return false;

	return ma_sound_at_end(sounds[soundID].get()) == MA_TRUE;
}

void AudioManager::StopSound(int32_t soundID)
{
	if (soundID == -1) return;

	ma_sound_stop(sounds[soundID].get());
}

bool AudioManager::IsStoppingSound(int32_t soundID)
{
	if (soundID == -1) return false;

	return !IsDoneSound(soundID) && !IsPlayingSound(soundID);
}

void AudioManager::ResetSound(int32_t soundID)
{
	if (soundID == -1) return;

	ma_sound_seek_to_pcm_frame(sounds[soundID].get(), 0);
}

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}