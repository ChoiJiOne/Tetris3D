#include "AudioManager.h"
#include "Sound.h"

Sound::Sound(const std::string& path)
{
	soundID_ = AudioManager::Get().CreateSound(path);
	CHECK((soundID_ != -1), "failed to create sound resource...");
}

Sound::~Sound()
{
	Stop();
}

void Sound::SetVolume(float volume)
{
	AudioManager::Get().SetSoundVolume(soundID_, volume);
}

float Sound::GetVolume()
{
	return AudioManager::Get().GetSoundVolume(soundID_);
}

void Sound::SetLooping(bool bIsLoop)
{
	AudioManager::Get().SetSoundLooping(soundID_, bIsLoop);
}

bool Sound::IsLooping()
{
	return AudioManager::Get().IsSoundLooping(soundID_);
}

void Sound::Play()
{
	AudioManager::Get().PlaySound(soundID_);
}

bool Sound::IsPlaying()
{
	return AudioManager::Get().IsPlayingSound(soundID_);
}

bool Sound::IsDone()
{
	return AudioManager::Get().IsDoneSound(soundID_);
}

void Sound::Stop()
{
	AudioManager::Get().StopSound(soundID_);
}

bool Sound::IsStopping()
{
	return AudioManager::Get().IsStoppingSound(soundID_);
}

void Sound::Reset()
{
	AudioManager::Get().ResetSound(soundID_);
}
