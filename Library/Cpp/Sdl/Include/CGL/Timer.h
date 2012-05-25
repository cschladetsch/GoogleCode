// (C) 2009 christian.schladetsch@gmail.com

#ifndef CGL_TIMER_H
#define CGL_TIMER_H

CGL_BEGIN

/// high-resolution timer. TODO: make cross-platform
struct Timer
{
private:
	static __int64 frequency;
	__int64 start_time, stop_time;

public:
	Timer(bool start = false);

	static void Initialise();

	void Start();
	void Stop();
	
	double GetElapsedMillis() const;
};

/// accumulating timer
struct Profiler
{
	Timer timer;
	double elapsed;

public:
	Profiler() : elapsed(0)
	{
	}
	void Start()
	{
		timer.Start();
	}
	void Stop()
	{
		timer.Stop();
		elapsed += timer.GetElapsedMillis();
	}
	void Reset()
	{
		timer.Stop();
		elapsed = 0;
	}

	double GetElapsedMillis() const
	{
		return elapsed;
	}
};

/// used to start/stop a profiler automatically using scope
struct BlockProfiler
{
	Profiler &profiler;

public:
	BlockProfiler(Profiler &pro) : profiler(pro)
	{
		profiler.Start();
	}
	~BlockProfiler()
	{
		profiler.Stop();
	}
};

CGL_END

#endif // CGL_TIMER_H

//EOF
