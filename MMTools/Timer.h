#pragma once

#include "Typedefs.h"

namespace MMT
{
	class Timer
	{
	private:
		u32 CurrentTicks = 0u;
		bool bIsStarted = false;
	public:
		//Timer();

		/// <summary>Returns the number of ticks elapsed since the last time 
		/// it was called; if the timer is paused, this number won't change 
		/// between calls. </summary>
		u32 GetTicks();
		void Start();
		void Pause();
	};
}