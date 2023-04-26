#include "GameTimer.h"

// @third party code - BEGIN SDL2
#include <SDL2/SDL.h>
// @third party code - END

float GameTimer::GetDeltaSeconds() const
{
	if (bIsStop_)
	{
		return 0.0f;
	}
	else
	{
		return static_cast<float>(currTime_ - prevTime_) / 1000.0f;
	}
}

float GameTimer::GetTotalSeconds() const
{
	if (bIsStop_)
	{
		return static_cast<float>(stopTime_ - pausedTime_ - baseTime_) / 1000.0f;
	}
	else
	{
		return static_cast<float>(currTime_ - pausedTime_ - baseTime_) / 1000.0f;
	}
}

void GameTimer::Reset()
{
	uint64_t tickTime = SDL_GetTicks64();

	bIsStop_ = false;
	baseTime_ = tickTime;
	pausedTime_ = 0ULL;
	stopTime_ = 0ULL;
	prevTime_ = tickTime;
	currTime_ = tickTime;
}

void GameTimer::Start()
{
	if (bIsStop_)
	{
		uint64_t tickTime = SDL_GetTicks64();

		pausedTime_ += (tickTime - stopTime_);
		prevTime_ = tickTime;
		stopTime_ = 0ULL;

		bIsStop_ = false;
	}
}

void GameTimer::Stop()
{
	if (!bIsStop_)
	{
		uint64_t tickTime = SDL_GetTicks64();

		stopTime_ = tickTime;

		bIsStop_ = true;
	}
}

void GameTimer::Tick()
{
	prevTime_ = currTime_;
	currTime_ = SDL_GetTicks64();
}