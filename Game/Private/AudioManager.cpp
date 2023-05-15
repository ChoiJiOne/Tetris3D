#include <algorithm>
#include <memory>
#include <vector>

#include "AudioManager.h"

// @third party code - BEGIN
#include <miniaudio/miniaudio.h>
// @third party code - END

void AudioManager::Setup()
{
	if (bIsSetup_) return;

	bIsSetup_ = true;
}

void AudioManager::Cleanup()
{
	if (!bIsSetup_) return;
	
	bIsSetup_ = false;
}

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}
