#pragma once
//https://www.daniweb.com/programming/software-development/code/428040/simple-timer-class-c

#ifndef TIMER_H
#define TIMER_H
#include <ctime>
class timer
{
	clock_t startedAt;
	clock_t pausedAt;
	bool started;
	bool paused;
public:
	timer();
	bool IsStarted();
	bool IsStopped();
	bool IsPaused();
	bool IsActive();
	void Pause();
	void Resume();
	void Stop();
	void Start();
	void Reset();
	clock_t GetTicks();
	clock_t GetMilliseconds();
	clock_t GetSeconds();
};


clock_t timer::GetMilliseconds()
{
	//if (!started)	return 0;
	if constexpr (CLOCKS_PER_SEC == 1000)
	{
		if (paused)  	return pausedAt - startedAt;
		return clock() - startedAt;
	}
	else
	{
		if (paused)  	return CLOCKS_PER_SEC * (pausedAt - startedAt)/1000;
		return CLOCKS_PER_SEC * (clock() - startedAt)/1000;
	}
}
clock_t timer::GetSeconds()
{
	return GetMilliseconds() / 1000;
}

clock_t timer::GetTicks()
{
	//if (!started)	return 0;

	if constexpr (CLOCKS_PER_SEC == 0)
	{
		if (paused)  	return pausedAt - startedAt;
		return clock() - startedAt;
	}
	else
	{
		if (paused)  	return CLOCKS_PER_SEC * (pausedAt - startedAt);
		return CLOCKS_PER_SEC * (clock() - startedAt);
	}

}





timer::timer()
{
	startedAt = 0;
	pausedAt = 0;
	paused = false;
	started = false;
}
bool timer::IsStarted()
{
	return started;
}
bool timer::IsStopped()
{
	return !started;
}
bool timer::IsPaused()
{
	return paused;
}
bool timer::IsActive()
{
	return (!paused) & started;
}
void timer::Pause()
{
	if (paused || !started)
		return;
	paused = true;
	pausedAt = clock();
}
void timer::Resume()
{
	if (!paused)
		return;
	paused = false;
	startedAt += clock() - pausedAt;
}
void timer::Stop()
{
	started = false;
}
void timer::Start()
{
	if (started)
		return;
	started = true;
	paused = false;
	startedAt = clock();
}
void timer::Reset()
{
	paused = false;
	startedAt = clock();
}



#endif


