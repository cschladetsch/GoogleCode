// (C) 2009 christian.schladetsch@gmail.com

#include "CGL/Precompiled.h"
#include "CGL/Timer.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

CGL_BEGIN

__int64 Timer::frequency;

Timer::Timer(bool start)
{
	if (start)
		Start();
}

void Timer::Initialise()
{
	QueryPerformanceFrequency((LARGE_INTEGER *) &frequency);
}

void Timer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&start_time);
}

void Timer::Stop()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&stop_time);
}

double Timer::GetElapsedMillis() const
{
	return (stop_time - start_time)*1000.0/frequency;
}

CGL_END

//EOF

