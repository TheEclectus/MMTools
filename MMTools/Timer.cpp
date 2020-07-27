#include "Timer.h"

namespace MMT
{
	u32 Timer::GetTicks()
	{
		u32 ToReturn = CurrentTicks - SDL_GetTicks();
		CurrentTicks = SDL_GetTicks();
		return ToReturn;
	}

	void Timer::Start()
	{
		bIsStarted = true;
		CurrentTicks = SDL_GetTicks();
	}

	void Timer::Pause()
	{
		bIsStarted = false;
	}
}